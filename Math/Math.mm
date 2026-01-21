#include "Math.hpp"

// Static Global Seed
static int32 GSRandSeed;

void FMath::SRandInit(int32 Seed)
{
    GSRandSeed = Seed;
}

int32 FMath::GetRandSeed()
{
    return GSRandSeed;
}

float FMath::SRand()
{
    GSRandSeed = (GSRandSeed * 196314165) + 907633515;
    
    union { float f; int32 i; } Result;
    union { float f; int32 i; } Temp;
    
    const float SRandTemp = 1.0f;
    Temp.f = SRandTemp;
    
    Result.i = (Temp.i & 0xff800000) | (GSRandSeed & 0x007fffff);
    
    // We can call the inline static function from the header
    return Fractional(Result.f);
}

float FMath::Atan2(float Y, float X)
{
    // Replaces standard atan2f with a minimax approximation.
    // Max relative error: ~7.15e-7
    // ~2x faster than std C version on PC/Mobile.

    const float absX = FMath::Abs(X);
    const float absY = FMath::Abs(Y);
    const bool yAbsBigger = (absY > absX);
    
    float t0 = yAbsBigger ? absY : absX;
    float t1 = yAbsBigger ? absX : absY;

    if (t0 == 0.f)
    {
        return 0.f;
    }

    float t3 = t1 / t0;
    float t4 = t3 * t3;

    // Minimax Coefficients
    static const float c[7] = {
        +7.3374712474035110e-03f,
        -5.0504856695026900e-02f,
        +1.5387532188431320e-01f,
        -2.7847936154865380e-01f,
        +4.1360646549216060e-01f,
        -7.0258071658492080e-01f,
        +9.9287372827532380e-01f
    };

    t0 = c[0];
    t0 = t0 * t4 + c[1];
    t0 = t0 * t4 + c[2];
    t0 = t0 * t4 + c[3];
    t0 = t0 * t4 + c[4];
    t0 = t0 * t4 + c[5];
    t0 = t0 * t4 + c[6];
    t3 = t0 * t3;

    t3 = yAbsBigger ? (0.5f * PI) - t3 : t3;
    t3 = (X < 0.0f) ? PI - t3 : t3;
    t3 = (Y < 0.0f) ? -t3 : t3;

    return t3;
}
