//
//  Memory.h
//  Asura
//
//  Created by Eux on 1/21/26.
//

#pragma once

#include <mach-o/dyld.h>
#include <mach-o/loader.h>
#include <mach/mach.h>
#include <mach/mach_init.h>
#include <mach/vm_map.h>
#include <type_traits>
#include <utility>
#include "Macros.h"
#include "Core.h"
#include "StringUtils.h"

class IMemoryUtils
{
public:
        
    static const IMemoryUtils* Get()
    {
        static IMemoryUtils StaticInstance;
        return &StaticInstance;
    }
    
public:
    template<typename Type>
    inline Type Read(uintptr_t Address) const
    {
        Type OutData;
        VMRead_Impl(Address, reinterpret_cast<void *>(&OutData), sizeof(Type));
        return OutData;
    }
    
    template<typename Type>
    inline void Write(uintptr_t Address, Type Data) const
    {
        VMWrite_Impl(Address, reinterpret_cast<void *>(&Data), sizeof(Type));
    }
    
    inline bool IsBadReadPtr(const void* Ptr) const
    {
        uint8_t Data = 0;
        size_t Size = 0;
        
        kern_return_t KR = vm_read_overwrite(mach_task_self(), (vm_address_t)Ptr, 1, (vm_address_t)&Data, &Size);
        return (KR == KERN_INVALID_ADDRESS ||
                KR == KERN_MEMORY_FAILURE  ||
                KR == KERN_MEMORY_ERROR    ||
                KR == KERN_PROTECTION_FAILURE);
    };
    
    inline bool IsBadReadPtr(const uintptr_t Ptr) const
    {
        return IsBadReadPtr(reinterpret_cast<const void*>(Ptr));
    }
    
    void VirtualProtect(uintptr_t Address, uintptr_t Size, vm_prot_t NewProtection) const
    {
        vm_protect(mach_task_self(), Address, Size, NO, NewProtection);
    }
    
public:
    
    struct DyldInfo
    {
        const mach_header_64 *Header;
        intptr VMAddrSlide;
        uint32 Index;
        const char* Path;
        
        DyldInfo() : Header(nullptr), VMAddrSlide(NULL), Index(-1), Path(nullptr) {}
        DyldInfo(const char* _Name) : Header(nullptr), VMAddrSlide(NULL), Index(-1)
        {
            for (uint32 Index = 0; Index < _dyld_image_count(); ++Index)
            {
                const char* ImageName = _dyld_get_image_name(Index);
                if (UTF8Utils::Strstr(ImageName, _Name))
                {
                    this->Index = Index;
                    this->Header = (const mach_header_64*)_dyld_get_image_header(Index);
                    this->VMAddrSlide = _dyld_get_image_vmaddr_slide(Index);
                    this->Path = ImageName;
                    
                    break;
                }
            }
        }
        
        void *GetAddress(uint64 Offset) const
        {
            return reinterpret_cast<void*>(GetBaseAddress() + Offset);
        }
        
        uint64 GetOffset(void *Address) const
        {
            return reinterpret_cast<uint64>(Address) - GetBaseAddress();
        }
        
        intptr GetBaseAddress() const
        {
            return VMAddrSlide;
        }
    };
    
    static DyldInfo GetDyldInfo(const char* Name)
    {
        return DyldInfo(Name);
    }
    
private:
    bool VMRead_Impl(long Address, void *buffer, int len) const
    {
        if (IsBadReadPtr(Address))
            return false;

        vm_size_t size = 0;
        kern_return_t error = vm_read_overwrite(mach_task_self(), (vm_address_t)Address, len, (vm_address_t)buffer, &size);
        return error == KERN_SUCCESS && size == len;
    }

    bool VMWrite_Impl(long Address, void *buffer, int len) const
    {
        if (IsBadReadPtr(Address))
            return false;

        kern_return_t error = vm_write(mach_task_self(), (vm_address_t)Address, (vm_offset_t)buffer, (mach_msg_type_number_t)len);
        return error == KERN_SUCCESS;
    }
};
