#pragma once

#include <cmath>
#include <cstdlib> 
#include "../Utils/Macros.h"
#include "../Utils/Core.h"

#undef  PI
#define PI                     (3.1415926535897932f)
#define SMALL_NUMBER           (1.e-8f)
#define KINDA_SMALL_NUMBER     (1.e-4f)
#define BIG_NUMBER             (3.4e+38f)
#define EULERS_NUMBER          (2.71828182845904523536f)
#define UE_GOLDEN_RATIO        (1.6180339887498948482045868343656381f)
#define FLOAT_NON_FRACTIONAL   (8388608.f)
#define MAX_FLT                3.402823466e+38F

#define INV_PI                 (0.31830988618f)
#define HALF_PI                (1.57079632679f)

#define UE_SQRT_2              (1.4142135623730950488016887242097f)
#define UE_SQRT_3              (1.7320508075688772935274463415059f)
#define UE_INV_SQRT_2          (0.70710678118654752440084436210485f)
#define UE_INV_SQRT_3          (0.57735026918962576450914878050196f)
#define UE_HALF_SQRT_2         (0.70710678118654752440084436210485f)
#define UE_HALF_SQRT_3         (0.86602540378443864676372317075294f)

#define DELTA                  (0.00001f)
#define RAD_TO_DEG             (180.f / PI)
#define DEG_TO_RAD             (PI / 180.f)

struct FMath
{
    static constexpr FORCEINLINE int32 TruncToInt(float F) { return (int32)F; }
    static FORCEINLINE float TruncToFloat(float F) { return truncf(F); }
    static FORCEINLINE int32 FloorToInt(float F) { return TruncToInt(floorf(F)); }
    static FORCEINLINE float FloorToFloat(float F) { return floorf(F); }
    static FORCEINLINE int32 RoundToInt(float F) { return FloorToInt(F + 0.5f); }
    static FORCEINLINE float RoundToFloat(float F) { return FloorToFloat(F + 0.5f); }
    static FORCEINLINE int32 CeilToInt(float F) { return TruncToInt(ceilf(F)); }
    static FORCEINLINE float CeilToFloat(float F) { return ceilf(F); }
    static FORCEINLINE float Fractional(float Value) { return Value - TruncToFloat(Value); }
    static FORCEINLINE float Frac(float Value) { return Value - FloorToFloat(Value); }
    static FORCEINLINE float Modf(const float InValue, float* OutIntPart) { return modff(InValue, OutIntPart); }

    static FORCEINLINE float Exp(float Value) { return expf(Value); }
    static FORCEINLINE float Exp2(float Value) { return powf(2.f, Value); }
    static FORCEINLINE float Loge(float Value) { return logf(Value); }
    static FORCEINLINE float LogX(float Base, float Value) { return Loge(Value) / Loge(Base); }
    static FORCEINLINE float Log2(float Value) { return Loge(Value) * 1.4426950f; }

    static FORCEINLINE float Fmod(float X, float Y)
    {
        if (fabsf(Y) <= 1.e-8f) return 0.f;
        const float Div = (X / Y);
        const float Quotient = fabsf(Div) < FLOAT_NON_FRACTIONAL ? TruncToFloat(Div) : Div;
        float IntPortion = Y * Quotient;
        if (fabsf(IntPortion) > fabsf(X)) IntPortion = X;
        const float Result = X - IntPortion;
        return Result;
    }

    // =========================================================================
    // Trigonometry (Optimized)
    // =========================================================================

    static FORCEINLINE float Sin(float Value) { return sinf(Value); }
    static FORCEINLINE float Cos(float Value) { return cosf(Value); }
    static FORCEINLINE float Tan(float Value) { return tanf(Value); }
    static FORCEINLINE float Asin(float Value) { return asinf((Value < -1.f) ? -1.f : ((Value < 1.f) ? Value : 1.f)); }
    static FORCEINLINE float Acos(float Value) { return acosf((Value < -1.f) ? -1.f : ((Value < 1.f) ? Value : 1.f)); }
    static FORCEINLINE float Atan(float Value) { return atanf(Value); }
    
    // Implemented in Math.mm (Minimax Approximation)
    static float Atan2(float Y, float X);

    static FORCEINLINE float Sqrt(float Value) { return sqrtf(Value); }
    static FORCEINLINE float Pow(float A, float B) { return powf(A, B); }
    static FORCEINLINE float InvSqrt(float F) { return 1.0f / sqrtf(F); }
    
    // Simultaneous Sin/Cos (Faster than calling them separately)
    static FORCEINLINE void SinCos(float* ScalarSin, float* ScalarCos, float Value)
    {
        // Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
        float quotient = (INV_PI * 0.5f) * Value;
        if (Value >= 0.0f) quotient = (float)((int)(quotient + 0.5f));
        else quotient = (float)((int)(quotient - 0.5f));
        float y = Value - (2.0f * PI) * quotient;

        // Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
        float sign;
        if (y > HALF_PI) { y = PI - y; sign = -1.0f; }
        else if (y < -HALF_PI) { y = -PI - y; sign = -1.0f; }
        else { sign = +1.0f; }

        float y2 = y * y;

        // 11-degree minimax approximation for Sin
        *ScalarSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

        // 10-degree minimax approximation for Cos
        float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
        *ScalarCos = sign * p;
    }

    // =========================================================================
    // Randomness
    // =========================================================================

    static FORCEINLINE int32 Rand() { return rand(); }
    static FORCEINLINE void RandInit(int32 Seed) { srand(Seed); }
    static FORCEINLINE float FRand() { return (Rand() & 0x7FFF) / (float)0x7FFF; } // Portable rand
    
    // Custom Seeded Random (Implemented in Math.mm)
    static void SRandInit(int32 Seed);
    static int32 GetRandSeed();
    static float SRand();

    static FORCEINLINE float FRandRange(float InMin, float InMax) { return InMin + (InMax - InMin) * FRand(); }
    static FORCEINLINE int32 RandRange(int32 Min, int32 Max) { return Min + (Rand() % ((Max - Min) + 1)); }
    static FORCEINLINE bool RandBool() { return (Rand() & 1); }

    template<class T> static constexpr FORCEINLINE T Abs(const T A) { return (A >= (T)0) ? A : -A; }
    template<class T> static constexpr FORCEINLINE T Sign(const T A) { return (A > (T)0) ? (T)1 : ((A < (T)0) ? (T)-1 : (T)0); }
    template<class T> static constexpr FORCEINLINE T Max(const T A, const T B) { return (A >= B) ? A : B; }
    template<class T> static constexpr FORCEINLINE T Min(const T A, const T B) { return (A <= B) ? A : B; }
    template<class T> static constexpr FORCEINLINE T Max3(const T A, const T B, const T C) { return Max(Max(A, B), C); }
    template<class T> static constexpr FORCEINLINE T Min3(const T A, const T B, const T C) { return Min(Min(A, B), C); }
    template<class T> static constexpr FORCEINLINE T Square(const T A) { return A * A; }
    template<class T> static constexpr FORCEINLINE T Clamp(const T X, const T Min, const T Max) { return X < Min ? Min : X < Max ? X : Max; }

    static FORCEINLINE bool IsNearlyEqual(float A, float B, float ErrorTolerance = SMALL_NUMBER) { return Abs(A - B) <= ErrorTolerance; }
    static FORCEINLINE bool IsNearlyZero(float Value, float ErrorTolerance = SMALL_NUMBER) { return Abs(Value) <= ErrorTolerance; }

    template<class T, class U> static FORCEINLINE T Lerp(const T& A, const T& B, const U& Alpha) { return (T)(A + Alpha * (B - A)); }
    
    template<class T> static FORCEINLINE T InterpEaseIn(const T& A, const T& B, float Alpha, float Exp)
    {
        return Lerp<T>(A, B, Pow(Alpha, Exp));
    }
    
    template<class T> static FORCEINLINE T InterpEaseOut(const T& A, const T& B, float Alpha, float Exp)
    {
        return Lerp<T>(A, B, 1.f - Pow(1.f - Alpha, Exp));
    }

    template<class T> static FORCEINLINE T InterpEaseInOut(const T& A, const T& B, float Alpha, float Exp)
    {
        return Lerp<T>(A, B, (Alpha < 0.5f) ? InterpEaseIn(0.f, 1.f, Alpha * 2.f, Exp) * 0.5f : InterpEaseOut(0.f, 1.f, Alpha * 2.f - 1.f, Exp) * 0.5f + 0.5f);
    }

    template<class T> static FORCEINLINE auto RadiansToDegrees(T const& RadVal) -> decltype(RadVal * RAD_TO_DEG) { return RadVal * RAD_TO_DEG; }
    template<class T> static FORCEINLINE auto DegreesToRadians(T const& DegVal) -> decltype(DegVal * DEG_TO_RAD) { return DegVal * DEG_TO_RAD; }

    static float UnwindDegrees(float A)
    {
        while (A > 180.f) A -= 360.f;
        while (A < -180.f) A += 360.f;
        return A;
    }

    static float UnwindRadians(float A)
    {
        while (A > PI) A -= (PI * 2.0f);
        while (A < -PI) A += (PI * 2.0f);
        return A;
    }

    static float FindDeltaAngleDegrees(float A1, float A2) { return UnwindDegrees(A2 - A1); }
    static float FindDeltaAngleRadians(float A1, float A2) { return UnwindRadians(A2 - A1); }

    static FORCEINLINE uint32 CountLeadingZeros(uint32 Value) { return (Value == 0) ? 32 : __builtin_clz(Value); }
    static FORCEINLINE uint64 CountLeadingZeros64(uint64 Value) { return (Value == 0) ? 64 : __builtin_clzll(Value); }
    static FORCEINLINE uint32 CountTrailingZeros(uint32 Value) { return (Value == 0) ? 32 : __builtin_ctz(Value); }
    static FORCEINLINE uint32 FloorLog2(uint32 Value) { return (31 - CountLeadingZeros(Value)); }
    static FORCEINLINE bool IsPowerOfTwo(uint32 Value) { return ((Value & (Value - 1)) == 0); }
    static FORCEINLINE uint32 CeilLogTwo(uint32 Arg)
    {
        int32 Bitmask = ((int32)(CountLeadingZeros(Arg) << 26)) >> 31;
        return (32 - CountLeadingZeros(Arg - 1)) & (~Bitmask);
    }
};
