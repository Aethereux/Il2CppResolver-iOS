#pragma once

#include "Math.hpp"
#include "Vector3.h"
#include <string>

#pragma clang diagnostic ignored "-Wint-to-pointer-cast"

struct Quaternion
{
    union
    {
        struct
        {
            float X;
            float Y;
            float Z;
            float W;
        };
        float data[4];
    };

    inline Quaternion() : X(0), Y(0), Z(0), W(1) {}
    inline Quaternion(float data[]) : X(data[0]), Y(data[1]), Z(data[2]), W(data[3]) {}
    inline Quaternion(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}
    inline Quaternion(Vector3 vector, float scalar) : X(vector.X), Y(vector.Y), Z(vector.Z), W(scalar) {}

    static inline Quaternion Identity() { return Quaternion(0, 0, 0, 1); }

    inline Quaternion& operator+=(const Quaternion rhs) { X += rhs.X; Y += rhs.Y; Z += rhs.Z; W += rhs.W; return *this; }
    inline Quaternion& operator-=(const Quaternion rhs) { X -= rhs.X; Y -= rhs.Y; Z -= rhs.Z; W -= rhs.W; return *this; }
    inline Quaternion& operator*=(const float rhs) { X *= rhs; Y *= rhs; Z *= rhs; W *= rhs; return *this; }
    inline Quaternion& operator/=(const float rhs) { float inv = 1.0f / rhs; X *= inv; Y *= inv; Z *= inv; W *= inv; return *this; }
    
    inline Quaternion operator+(const Quaternion& rhs) const { return Quaternion(X + rhs.X, Y + rhs.Y, Z + rhs.Z, W + rhs.W); }
    inline Quaternion operator-(const Quaternion& rhs) const { return Quaternion(X - rhs.X, Y - rhs.Y, Z - rhs.Z, W - rhs.W); }
    
    // Fix: operator* and operator/ defined as members
    inline Quaternion operator*(float rhs) const { return Quaternion(X * rhs, Y * rhs, Z * rhs, W * rhs); }
    inline Quaternion operator/(float rhs) const { float inv = 1.0f / rhs; return Quaternion(X * inv, Y * inv, Z * inv, W * inv); }
    
    // Quaternion multiplication
    inline Quaternion operator*(const Quaternion& rhs) const
    {
        return Quaternion(
            W * rhs.X + X * rhs.W + Y * rhs.Z - Z * rhs.Y,
            W * rhs.Y + Y * rhs.W + Z * rhs.X - X * rhs.Z,
            W * rhs.Z + Z * rhs.W + X * rhs.Y - Y * rhs.X,
            W * rhs.W - X * rhs.X - Y * rhs.Y - Z * rhs.Z
        );
    }
    
    inline Quaternion operator-() const { return Quaternion(-X, -Y, -Z, -W); }
    inline bool operator==(const Quaternion& rhs) const { return X == rhs.X && Y == rhs.Y && Z == rhs.Z && W == rhs.W; }
    inline bool operator!=(const Quaternion& rhs) const { return !(*this == rhs); }

    static inline float Dot(Quaternion lhs, Quaternion rhs)
    {
        return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z + lhs.W * rhs.W;
    }

    static inline Quaternion Conjugate(Quaternion rotation)
    {
        return Quaternion(-rotation.X, -rotation.Y, -rotation.Z, rotation.W);
    }

    static inline Quaternion Inverse(Quaternion rotation)
    {
        float n = Dot(rotation, rotation);
        if (n < SMALL_NUMBER) return Quaternion::Identity();
        return Conjugate(rotation) / n; // This works now because operator/ is a member
    }

    static inline Quaternion Slerp(Quaternion a, Quaternion b, float t)
    {
        return SlerpUnclamped(a, b, FMath::Clamp(t, 0.0f, 1.0f));
    }

    static inline Quaternion SlerpUnclamped(Quaternion a, Quaternion b, float t)
    {
        float dt = Dot(a, b);
        if (dt < 0.0f) { dt = -dt; b = b * -1.0f; } // Negate for shortest path

        if (dt < 0.9995f) {
            float angle = FMath::Acos(dt);
            float sinAngle = FMath::Sqrt(1.0f - dt * dt);
            float invSinAngle = 1.0f / sinAngle;
            float coeff0 = FMath::Sin((1.0f - t) * angle) * invSinAngle;
            float coeff1 = FMath::Sin(t * angle) * invSinAngle;
            return a * coeff0 + b * coeff1;
        } else {
            return Normalize(a + (b - a) * t);
        }
    }
    
    static inline Quaternion Normalize(Quaternion rotation)
    {
        float n = Dot(rotation, rotation);
        if (n < SMALL_NUMBER) return Quaternion::Identity();
        return rotation / FMath::Sqrt(n);
    }

    static inline Quaternion Euler(float x, float y, float z)
    {
        float cX, sX, cY, sY, cZ, sZ;
        FMath::SinCos(&sX, &cX, x * 0.5f * DEG_TO_RAD);
        FMath::SinCos(&sY, &cY, y * 0.5f * DEG_TO_RAD);
        FMath::SinCos(&sZ, &cZ, z * 0.5f * DEG_TO_RAD);

        Quaternion q;
        // Unity Order: ZXY
        q.X = (cY * sX * cZ) + (sY * cX * sZ);
        q.Y = (sY * cX * cZ) - (cY * sX * sZ);
        q.Z = (cY * cX * sZ) - (sY * sX * cZ);
        q.W = (cY * cX * cZ) + (sY * sX * sZ);
        return q;
    }

    static inline Quaternion LookRotation(Vector3 forward, Vector3 upwards = Vector3::Up())
    {
        float mag = Vector3::Magnitude(forward);
        if (mag < SMALL_NUMBER) return Quaternion::Identity();
        forward /= mag;
        
        Vector3 right = Vector3::Cross(upwards, forward);
        float len = Vector3::Magnitude(right);
        if (len < SMALL_NUMBER) return Quaternion::Identity();
            
        right /= len;
        Vector3 up = Vector3::Cross(forward, right);
        
        float m00 = right.X, m01 = right.Y, m02 = right.Z;
        float m10 = up.X,    m11 = up.Y,    m12 = up.Z;
        float m20 = forward.X, m21 = forward.Y, m22 = forward.Z;

        float trace = m00 + m11 + m22;
        Quaternion q;

        if (trace > 0.0f) {
            float s = FMath::Sqrt(trace + 1.0f);
            q.W = s * 0.5f;
            s = 0.5f / s;
            q.X = (m12 - m21) * s;
            q.Y = (m20 - m02) * s;
            q.Z = (m01 - m10) * s;
        } else {
            if (m00 >= m11 && m00 >= m22) {
                float s = FMath::Sqrt(1.0f + m00 - m11 - m22);
                float invS = 0.5f / s;
                q.X = 0.5f * s;
                q.Y = (m01 + m10) * invS;
                q.Z = (m02 + m20) * invS;
                q.W = (m12 - m21) * invS;
            } else if (m11 > m22) {
                float s = FMath::Sqrt(1.0f + m11 - m00 - m22);
                float invS = 0.5f / s;
                q.X = (m10 + m01) * invS;
                q.Y = 0.5f * s;
                q.Z = (m21 + m12) * invS;
                q.W = (m20 - m02) * invS;
            } else {
                float s = FMath::Sqrt(1.0f + m22 - m00 - m11);
                float invS = 0.5f / s;
                q.X = (m20 + m02) * invS;
                q.Y = (m21 + m12) * invS;
                q.Z = 0.5f * s;
                q.W = (m01 - m10) * invS;
            }
        }
        return q;
    }

    static inline std::string ToString(Quaternion a)
    {
        return "(" + std::to_string(a.X) + ", " + std::to_string(a.Y) + ", " + std::to_string(a.Z) + ", " + std::to_string(a.W) + ")";
    }
};

// Global: Rotate Vector by Quaternion
inline Vector3 operator*(const Quaternion& lhs, const Vector3& rhs)
{
    float x = lhs.X * 2.0f;
    float y = lhs.Y * 2.0f;
    float z = lhs.Z * 2.0f;
    float xx = lhs.X * x;
    float yy = lhs.Y * y;
    float zz = lhs.Z * z;
    float xy = lhs.X * y;
    float xz = lhs.X * z;
    float yz = lhs.Y * z;
    float wx = lhs.W * x;
    float wy = lhs.W * y;
    float wz = lhs.W * z;

    Vector3 res;
    res.X = (1.0f - (yy + zz)) * rhs.X + (xy - wz) * rhs.Y + (xz + wy) * rhs.Z;
    res.Y = (xy + wz) * rhs.X + (1.0f - (xx + zz)) * rhs.Y + (yz - wx) * rhs.Z;
    res.Z = (xz - wy) * rhs.X + (yz + wx) * rhs.Y + (1.0f - (xx + yy)) * rhs.Z;
    return res;
}

