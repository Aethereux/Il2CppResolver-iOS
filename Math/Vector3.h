#pragma once

#include "Math.hpp"
#include <string>
#include <string.h>

#pragma clang diagnostic ignored "-Wint-to-pointer-cast"


struct Vector3
{
    union
    {
        struct
        {
            float X;
            float Y;
            float Z;
        };
        float data[3];
    };

    // Constructors
    inline Vector3() : X(0), Y(0), Z(0) {}
    inline Vector3(float data[]) : X(data[0]), Y(data[1]), Z(data[2]) {}
    inline Vector3(float value) : X(value), Y(value), Z(value) {}
    inline Vector3(float x, float y) : X(x), Y(y), Z(0) {}
    inline Vector3(float x, float y, float z) : X(x), Y(y), Z(z) {}

    static inline Vector3 Zero() { return Vector3(0, 0, 0); }
    static inline Vector3 One() { return Vector3(1, 1, 1); }
    static inline Vector3 Right() { return Vector3(1, 0, 0); }
    static inline Vector3 Left() { return Vector3(-1, 0, 0); }
    static inline Vector3 Up() { return Vector3(0, 1, 0); }
    static inline Vector3 Down() { return Vector3(0, -1, 0); }
    static inline Vector3 Forward() { return Vector3(0, 0, 1); }
    static inline Vector3 Backward() { return Vector3(0, 0, -1); }

    inline Vector3& operator+=(const float rhs) { X += rhs; Y += rhs; Z += rhs; return *this; }
    inline Vector3& operator-=(const float rhs) { X -= rhs; Y -= rhs; Z -= rhs; return *this; }
    inline Vector3& operator*=(const float rhs) { X *= rhs; Y *= rhs; Z *= rhs; return *this; }
    inline Vector3& operator/=(const float rhs) { float inv = 1.0f / rhs; X *= inv; Y *= inv; Z *= inv; return *this; }
    
    inline Vector3& operator+=(const Vector3 rhs) { X += rhs.X; Y += rhs.Y; Z += rhs.Z; return *this; }
    inline Vector3& operator-=(const Vector3 rhs) { X -= rhs.X; Y -= rhs.Y; Z -= rhs.Z; return *this; }
    
    inline Vector3& operator*=(const Vector3 rhs) { X *= rhs.X; Y *= rhs.Y; Z *= rhs.Z; return *this; }
    inline Vector3& operator/=(const Vector3 rhs) { X /= rhs.X; Y /= rhs.Y; Z /= rhs.Z; return *this; }

    inline Vector3 operator+(const Vector3& rhs) const { return Vector3(X + rhs.X, Y + rhs.Y, Z + rhs.Z); }
    inline Vector3 operator-(const Vector3& rhs) const { return Vector3(X - rhs.X, Y - rhs.Y, Z - rhs.Z); }
    inline Vector3 operator*(const Vector3& rhs) const { return Vector3(X * rhs.X, Y * rhs.Y, Z * rhs.Z); } // Element-wise
    inline Vector3 operator/(const Vector3& rhs) const { return Vector3(X / rhs.X, Y / rhs.Y, Z / rhs.Z); } // Element-wise

    inline Vector3 operator+(float rhs) const { return Vector3(X + rhs, Y + rhs, Z + rhs); }
    inline Vector3 operator-(float rhs) const { return Vector3(X - rhs, Y - rhs, Z - rhs); }
    inline Vector3 operator*(float rhs) const { return Vector3(X * rhs, Y * rhs, Z * rhs); }
    inline Vector3 operator/(float rhs) const { float inv = 1.0f / rhs; return Vector3(X * inv, Y * inv, Z * inv); }
    
    inline Vector3 operator-() const { return Vector3(-X, -Y, -Z); }

    inline bool operator==(const Vector3& rhs) const { return X == rhs.X && Y == rhs.Y && Z == rhs.Z; }
    inline bool operator!=(const Vector3& rhs) const { return !(*this == rhs); }

    static inline float SqrMagnitude(Vector3 v) { return v.X * v.X + v.Y * v.Y + v.Z * v.Z; }
    static inline float Magnitude(Vector3 v) { return FMath::Sqrt(SqrMagnitude(v)); }

    static inline Vector3 Normalized(Vector3 v)
    {
        float sqrMag = SqrMagnitude(v);
        if (sqrMag < SMALL_NUMBER) return Vector3::Zero();
        float scale = FMath::InvSqrt(sqrMag);
        return Vector3(v.X * scale, v.Y * scale, v.Z * scale);
    }

    static inline float Dot(Vector3 lhs, Vector3 rhs) { return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z; }

    static inline float Angle(Vector3 a, Vector3 b)
    {
        float denominator = (float)FMath::Sqrt(SqrMagnitude(a) * SqrMagnitude(b));
        if (denominator < KINDA_SMALL_NUMBER) return 0.0f;
        float dot = Dot(a, b) / denominator;
        dot = FMath::Clamp(dot, -1.0f, 1.0f);
        return FMath::Acos(dot);
    }

    static inline Vector3 ClampMagnitude(Vector3 vector, float maxLength)
    {
        float sqrMag = SqrMagnitude(vector);
        if (sqrMag > maxLength * maxLength) {
            float scale = maxLength * FMath::InvSqrt(sqrMag);
            return Vector3(vector.X * scale, vector.Y * scale, vector.Z * scale);
        }
        return vector;
    }

    static inline Vector3 Cross(Vector3 lhs, Vector3 rhs)
    {
        return Vector3(
            lhs.Y * rhs.Z - lhs.Z * rhs.Y,
            lhs.Z * rhs.X - lhs.X * rhs.Z,
            lhs.X * rhs.Y - lhs.Y * rhs.X
        );
    }

    static inline float Distance(Vector3 a, Vector3 b)
    {
        float dx = a.X - b.X; float dy = a.Y - b.Y; float dz = a.Z - b.Z;
        return FMath::Sqrt(dx*dx + dy*dy + dz*dz);
    }

    static inline Vector3 LerpUnclamped(Vector3 a, Vector3 b, float t)
    {
        return Vector3(a.X + (b.X - a.X) * t, a.Y + (b.Y - a.Y) * t, a.Z + (b.Z - a.Z) * t);
    }

    static inline Vector3 Lerp(Vector3 a, Vector3 b, float t)
    {
        return LerpUnclamped(a, b, FMath::Clamp(t, 0.0f, 1.0f));
    }

    static inline std::string ToString(Vector3 a)
    {
        return std::to_string(a.X) + ", " + std::to_string(a.Y) + ", " + std::to_string(a.Z);
    }
    
    // Re-implement Scale (Unity style) using element-wise mult
    static inline Vector3 Scale(Vector3 a, Vector3 b) { return a * b; }
    
    static inline Vector3 Project(Vector3 a, Vector3 b)
    {
        float sqrMag = SqrMagnitude(b);
        if (sqrMag < SMALL_NUMBER) return Vector3::Zero();
        return b * (Dot(a, b) / sqrMag);
    }
};

// Global operator for float * Vector3
inline Vector3 operator*(float lhs, const Vector3& rhs) { return rhs * lhs; }

