//
//  Vector2.h
//  Asura
//
//  Created by Eux on 1/21/26.
//

#pragma once

#include "Math.hpp"
#include <string>
#include <string.h>

struct Vector2
{
    union
    {
        struct
        {
            float X;
            float Y;
        };
        float data[2];
    };

    /**
     * Constructors.
     */
    FORCEINLINE Vector2() : X(0), Y(0) {}
    FORCEINLINE Vector2(float data[]) : X(data[0]), Y(data[1]) {}
    FORCEINLINE Vector2(float value) : X(value), Y(value) {}
    FORCEINLINE Vector2(float x, float y) : X(x), Y(y) {}
    
    FORCEINLINE Vector2 operator-() { return Vector2(-X, -Y); }
    
    /**
     * Constants for common vectors.
     * Unity 2D standard: Y is Up, X is Right.
     */
    static FORCEINLINE Vector2 Zero() { return Vector2(0, 0); }
    static FORCEINLINE Vector2 One() { return Vector2(1, 1); }
    static FORCEINLINE Vector2 Right() { return Vector2(1, 0); }
    static FORCEINLINE Vector2 Left() { return Vector2(-1, 0); }
    static FORCEINLINE Vector2 Up() { return Vector2(0, 1); }
    static FORCEINLINE Vector2 Down() { return Vector2(0, -1); }

    /**
     * Returns the squared magnitude of a vector.
     */
    static FORCEINLINE float SqrMagnitude(Vector2 v)
    {
        return v.X * v.X + v.Y * v.Y;
    }

    /**
     * Returns the magnitude of a vector.
     * Optimization: Uses FMath::Sqrt
     */
    static FORCEINLINE float Magnitude(Vector2 v)
    {
        return FMath::Sqrt(SqrMagnitude(v));
    }

    /**
     * Returns a new vector with magnitude of one.
     * Optimization: Uses Fast Inverse Square Root (InvSqrt)
     */
    static FORCEINLINE Vector2 Normalized(Vector2 v)
    {
        float sqrMag = SqrMagnitude(v);
        if (sqrMag < SMALL_NUMBER)
            return Vector2::Zero();
        
        float scale = FMath::InvSqrt(sqrMag);
        return Vector2(v.X * scale, v.Y * scale);
    }

    /**
     * Returns the dot product of two vectors.
     */
    static FORCEINLINE float Dot(Vector2 lhs, Vector2 rhs)
    {
        return lhs.X * rhs.X + lhs.Y * rhs.Y;
    }

    /**
     * Returns the angle between two vectors in radians.
     * Optimization: Uses FMath::Acos and Clamping
     */
    static FORCEINLINE float Angle(Vector2 a, Vector2 b)
    {
        float denominator = (float)FMath::Sqrt(SqrMagnitude(a) * SqrMagnitude(b));
        if (denominator < KINDA_SMALL_NUMBER)
            return 0.0f;

        float dot = Dot(a, b) / denominator;
        dot = FMath::Clamp(dot, -1.0f, 1.0f);
        return FMath::Acos(dot);
    }
    
    /**
     * Returns the signed angle in degrees between from and to.
     * (Positive if 'to' is to the left of 'from')
     */
    static FORCEINLINE float SignedAngle(Vector2 from, Vector2 to)
    {
        float unsigned_angle = Angle(from, to);
        float sign = FMath::Sign(from.X * to.Y - from.Y * to.X);
        return unsigned_angle * sign;
    }

    /**
     * Returns a vector with its magnitude clamped to maxLength.
     */
    static FORCEINLINE Vector2 ClampMagnitude(Vector2 vector, float maxLength)
    {
        float sqrMag = SqrMagnitude(vector);
        if (sqrMag > maxLength * maxLength)
        {
            float scale = maxLength * FMath::InvSqrt(sqrMag);
            return Vector2(vector.X * scale, vector.Y * scale);
        }
        return vector;
    }

    static FORCEINLINE float Component(Vector2 a, Vector2 b)
    {
        float magB = Magnitude(b);
        if (magB < SMALL_NUMBER) return 0.0f;
        return Dot(a, b) / magB;
    }

    static FORCEINLINE float Distance(Vector2 a, Vector2 b)
    {
        float dx = a.X - b.X;
        float dy = a.Y - b.Y;
        return FMath::Sqrt(dx*dx + dy*dy);
    }
    
    // FIX: Now returns a std::string to avoid returning invalid stack memory
    static FORCEINLINE std::string ToString(Vector2 a)
    {
        return std::to_string(a.X) + ", " + std::to_string(a.Y);
    }

    static FORCEINLINE Vector2 LerpUnclamped(Vector2 a, Vector2 b, float t)
    {
        return Vector2(
            a.X + (b.X - a.X) * t,
            a.Y + (b.Y - a.Y) * t
        );
    }

    static FORCEINLINE Vector2 Lerp(Vector2 a, Vector2 b, float t)
    {
        return LerpUnclamped(a, b, FMath::Clamp(t, 0.0f, 1.0f));
    }

    static FORCEINLINE Vector2 Max(Vector2 a, Vector2 b)
    {
        return Vector2(FMath::Max(a.X, b.X), FMath::Max(a.Y, b.Y));
    }

    static FORCEINLINE Vector2 Min(Vector2 a, Vector2 b)
    {
        return Vector2(FMath::Min(a.X, b.X), FMath::Min(a.Y, b.Y));
    }

    static FORCEINLINE Vector2 MoveTowards(Vector2 current, Vector2 target, float maxDistanceDelta)
    {
        Vector2 d = target - current;
        float sqrMag = SqrMagnitude(d);
        
        if (sqrMag == 0.0f || (maxDistanceDelta >= 0.0f && sqrMag <= maxDistanceDelta * maxDistanceDelta))
            return target;
        
        float dist = FMath::Sqrt(sqrMag);
        return current + (d * (maxDistanceDelta / dist));
    }
    
    static FORCEINLINE Vector2 Reflect(Vector2 vector, Vector2 planeNormal)
    {
        return vector - planeNormal * (2.0f * Dot(vector, planeNormal));
    }
    
    static FORCEINLINE Vector2 Scale(Vector2 a, Vector2 b)
    {
        return Vector2(a.X * b.X, a.Y * b.Y);
    }
    
    // Operators
    FORCEINLINE struct Vector2& operator+=(const float rhs) { X += rhs; Y += rhs; return *this; }
    FORCEINLINE struct Vector2& operator-=(const float rhs) { X -= rhs; Y -= rhs; return *this; }
    FORCEINLINE struct Vector2& operator*=(const float rhs) { X *= rhs; Y *= rhs; return *this; }
    FORCEINLINE struct Vector2& operator/=(const float rhs) { float inv = 1.0f / rhs; X *= inv; Y *= inv; return *this; }
    FORCEINLINE struct Vector2& operator+=(const Vector2 rhs) { X += rhs.X; Y += rhs.Y; return *this; }
    FORCEINLINE struct Vector2& operator-=(const Vector2 rhs) { X -= rhs.X; Y -= rhs.Y; return *this; }
    // Friend Operators
    friend FORCEINLINE Vector2 operator+(Vector2 lhs, const float rhs) { return Vector2(lhs.X + rhs, lhs.Y + rhs); }
    friend FORCEINLINE Vector2 operator-(Vector2 lhs, const float rhs) { return Vector2(lhs.X - rhs, lhs.Y - rhs); }
    friend FORCEINLINE Vector2 operator*(Vector2 lhs, const float rhs) { return Vector2(lhs.X * rhs, lhs.Y * rhs); }
    friend FORCEINLINE Vector2 operator/(Vector2 lhs, const float rhs) { return Vector2(lhs.X / rhs, lhs.Y / rhs); }
    friend FORCEINLINE Vector2 operator+(const float lhs, Vector2 rhs) { return Vector2(lhs + rhs.X, lhs + rhs.Y); }
    friend FORCEINLINE Vector2 operator-(const float lhs, Vector2 rhs) { return Vector2(lhs - rhs.X, lhs - rhs.Y); }
    friend FORCEINLINE Vector2 operator*(const float lhs, Vector2 rhs) { return Vector2(lhs * rhs.X, lhs * rhs.Y); }
    friend FORCEINLINE Vector2 operator/(const float lhs, Vector2 rhs) { return Vector2(lhs / rhs.X, lhs / rhs.Y); }
    friend FORCEINLINE Vector2 operator+(Vector2 lhs, const Vector2 rhs) { return Vector2(lhs.X + rhs.X, lhs.Y + rhs.Y); }
    friend FORCEINLINE Vector2 operator-(Vector2 lhs, const Vector2 rhs) { return Vector2(lhs.X - rhs.X, lhs.Y - rhs.Y); }
    friend FORCEINLINE bool operator==(const Vector2 lhs, const Vector2 rhs) { return lhs.X == rhs.X && lhs.Y == rhs.Y; }
    friend FORCEINLINE bool operator!=(const Vector2 lhs, const Vector2 rhs) { return !(lhs == rhs); }
};
