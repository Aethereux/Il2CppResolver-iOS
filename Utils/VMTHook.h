//
//  VMTHook.h
//  Sishen
//
//  Created by Zaraki on 18.07.25.
//

#include "Memory.h"


template<typename FuncType>
class VMTHookManager
{
private:
    FuncType* OriginalFunction;
    FuncType* NewFunction;

    int32 FunctionIndex;
public:
    VMTHookManager(FuncType* NewFunc, int32 Index) : NewFunction(NewFunc), FunctionIndex(Index) {}
    VMTHookManager(FuncType* NewFunc) : NewFunction(NewFunc) {}
    
    void VirtualProtect(uintptr_t Address, uintptr_t Size, vm_prot_t NewProtection) const
    {
        vm_protect(mach_task_self(), Address, Size, NO, NewProtection);
    }

    void Swap(void* Class)
    {
        if (!Class || !NewFunction)
            return;

        void** VTable = *reinterpret_cast<void***>(Class);
        if (!VTable || VTable[FunctionIndex] == NewFunction)
            return;

        OriginalFunction = (FuncType*)VTable[FunctionIndex];

        VirtualProtect((uint64)VTable, FunctionIndex * sizeof(void*), VM_PROT_READ | VM_PROT_WRITE | VM_PROT_COPY);
        VTable[FunctionIndex] = (void*)NewFunction;
        VirtualProtect((uint64)VTable, FunctionIndex * sizeof(void*), VM_PROT_READ);
    }

    void SwapInstanceless(void* AddressInVTable)
    {
        if (NewFunction == *(FuncType**)AddressInVTable)
            return;

        OriginalFunction = *(FuncType**)AddressInVTable;
        VirtualProtect((uint64)AddressInVTable, sizeof(void*), VM_PROT_READ | VM_PROT_WRITE | VM_PROT_COPY);
        *(void**)AddressInVTable = (void*)NewFunction;
        VirtualProtect((uint64)AddressInVTable, sizeof(void*), VM_PROT_READ);
    }

    void ResetInstanceless(void* AddressInVTable)
    {
        if (!OriginalFunction)
            return;

        if (NewFunction != *(FuncType**)AddressInVTable)
            return;

        VirtualProtect((uint64)AddressInVTable, sizeof(void*), VM_PROT_READ | VM_PROT_WRITE | VM_PROT_COPY);
        *(void**)AddressInVTable = (void*)OriginalFunction;
        VirtualProtect((uint64)AddressInVTable, sizeof(void*), VM_PROT_READ);
    }

    void Reset(void* Class)
    {
        if (!Class || !OriginalFunction)
            return;

        void** VTable = *reinterpret_cast<void***>(Class);
        if (!VTable || VTable[FunctionIndex] != NewFunction)
            return;

        VirtualProtect((uint64)VTable, sizeof(void*), VM_PROT_READ | VM_PROT_WRITE | VM_PROT_COPY);
        VTable[FunctionIndex] = (void*)OriginalFunction;
        VirtualProtect((uint64)VTable, sizeof(void*), VM_PROT_READ | VM_PROT_EXECUTE);
    }

    template<typename... Args>
    auto InvokeOriginal(Args&&... args) const -> decltype(auto)
    {
        return (*OriginalFunction)(std::forward<Args>(args)...);
    }
};
