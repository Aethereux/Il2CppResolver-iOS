//
//  Matrix4x4.h
//  Asura
//
//  Created by Euclid Jan Guillermo on 1/21/26.
//

#pragma once

#include "Math.hpp"
#include "Vector3.h"
#include <string.h>
// Forward declaration
struct Matrix4x4;
struct Vector4
{
    union
    {
        struct
        {
            float x, y, z, w;
        };
        float data[4];
    };

    inline Vector4() : x(0), y(0), z(0), w(0) {}
    inline Vector4(float f1, float f2, float f3, float f4) : x(f1), y(f2), z(f3), w(f4) {}
    inline Vector4(Vector3 v, float w_val) : x(v.X), y(v.Y), z(v.Z), w(w_val) {}

    // Constants
    static inline Vector4 Zero() { return Vector4(0, 0, 0, 0); }
    static inline Vector4 One() { return Vector4(1, 1, 1, 1); }

    static inline float Dot(Vector4 a, Vector4 b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    static inline float SqrMagnitude(Vector4 a)
    {
        return Dot(a, a);
    }

    static inline float Magnitude(Vector4 a)
    {
        return FMath::Sqrt(SqrMagnitude(a));
    }

    static inline Vector4 Normalize(Vector4 a)
    {
        float sqrMag = SqrMagnitude(a);
        if (sqrMag < SMALL_NUMBER)
            return Zero();
        float scale = FMath::InvSqrt(sqrMag);
        return Vector4(a.x * scale, a.y * scale, a.z * scale, a.w * scale);
    }

    static inline Vector4 Lerp(Vector4 a, Vector4 b, float t)
    {
        t = FMath::Clamp(t, 0.0f, 1.0f);
        return Vector4(
            a.x + (b.x - a.x) * t,
            a.y + (b.y - a.y) * t,
            a.z + (b.z - a.z) * t,
            a.w + (b.w - a.w) * t
        );
    }

    // Operators
    inline Vector4 operator+(const Vector4& b) const { return Vector4(x + b.x, y + b.y, z + b.z, w + b.w); }
    inline Vector4 operator-(const Vector4& b) const { return Vector4(x - b.x, y - b.y, z - b.z, w - b.w); }
    inline Vector4 operator*(float d) const { return Vector4(x * d, y * d, z * d, w * d); }
    inline Vector4 operator/(float d) const { float inv = 1.0f / d; return Vector4(x * inv, y * inv, z * inv, w * inv); }
    
    inline bool operator==(const Vector4& b) const { return x == b.x && y == b.y && z == b.z && w == b.w; }
    inline bool operator!=(const Vector4& b) const { return !(*this == b); }
};

// -----------------------------------------------------------------------------
// Simple Structs
// -----------------------------------------------------------------------------
struct Bounds
{
    Vector3 m_vCenter;
    Vector3 m_vExtents;
    
    Vector3 GetMin() const { return m_vCenter - m_vExtents; }
    Vector3 GetMax() const { return m_vCenter + m_vExtents; }
};

struct Plane
{
    Vector3 m_vNormal;
    float fDistance;
};

struct Ray
{
    Vector3 m_vOrigin;
    Vector3 m_vDirection;
    
    Vector3 GetPoint(float distance) const {
        return m_vOrigin + (m_vDirection * distance);
    }
};

struct Rect
{
    float fX, fY;
    float fWidth, fHeight;

    Rect() : fX(0), fY(0), fWidth(0), fHeight(0) {}
    Rect(float x, float y, float w, float h) : fX(x), fY(y), fWidth(w), fHeight(h) {}
    
    bool Contains(Vector2 point) const {
        return (point.X >= fX && point.X < fX + fWidth && point.Y >= fY && point.Y < fY + fHeight);
    }
};

struct Color
{
    float r, g, b, a;

    Color() : r(0), g(0), b(0), a(0) {}
    Color(float fRed, float fGreen, float fBlue, float fAlpha = 1.0f)
        : r(fRed), g(fGreen), b(fBlue), a(fAlpha) {}
        
    static Color Red() { return Color(1,0,0,1); }
    static Color Green() { return Color(0,1,0,1); }
    static Color Blue() { return Color(0,0,1,1); }
    static Color White() { return Color(1,1,1,1); }
    static Color Black() { return Color(0,0,0,1); }
};

// -----------------------------------------------------------------------------
// Matrix4x4
// Note: Unity uses Column-Major memory layout.
// -----------------------------------------------------------------------------
struct Matrix4x4
{
    union
    {
        // Standard float array (16 floats)
        float Data[16];
        
        // Explicit names matching Unity's Column-Major layout
        // mRowCol (e.g., m02 is Row 0, Col 2)
        struct {
            float m00, m10, m20, m30; // Column 0
            float m01, m11, m21, m31; // Column 1
            float m02, m12, m22, m32; // Column 2
            float m03, m13, m23, m33; // Column 3
        };
    };

    Matrix4x4() { memset(Data, 0, sizeof(Data)); }

    static inline Matrix4x4 Identity()
    {
        Matrix4x4 m;
        m.m00 = 1; m.m11 = 1; m.m22 = 1; m.m33 = 1;
        return m;
    }

    // Access element at Row, Col
    inline float Get(int row, int col) const
    {
        return Data[col * 4 + row];
    }

    inline void Set(int row, int col, float val)
    {
        Data[col * 4 + row] = val;
    }
    
    // Allows m[row] syntax, but be careful: this returns a pointer to the start of the COLUMN in memory.
    // It is safer to use Get/Set for logic.
    inline float operator()(int row, int col) const { return Get(row, col); }

    // Transforms a position (w = 1.0)
    inline Vector3 MultiplyPoint3x4(const Vector3& v) const
    {
        Vector3 res;
        res.X = m00 * v.X + m01 * v.Y + m02 * v.Z + m03;
        res.Y = m10 * v.X + m11 * v.Y + m12 * v.Z + m13;
        res.Z = m20 * v.X + m21 * v.Y + m22 * v.Z + m23;
        return res;
    }

    // Transforms a position with perspective divide
    inline Vector3 MultiplyPoint(const Vector3& v) const
    {
        Vector3 res;
        float w;
        res.X = m00 * v.X + m01 * v.Y + m02 * v.Z + m03;
        res.Y = m10 * v.X + m11 * v.Y + m12 * v.Z + m13;
        res.Z = m20 * v.X + m21 * v.Y + m22 * v.Z + m23;
        w     = m30 * v.X + m31 * v.Y + m32 * v.Z + m33;

        w = 1.0f / w;
        return Vector3(res.X * w, res.Y * w, res.Z * w);
    }

    // Transforms a direction (w = 0.0), ignoring translation
    inline Vector3 MultiplyVector(const Vector3& v) const
    {
        Vector3 res;
        res.X = m00 * v.X + m01 * v.Y + m02 * v.Z;
        res.Y = m10 * v.X + m11 * v.Y + m12 * v.Z;
        res.Z = m20 * v.X + m21 * v.Y + m22 * v.Z;
        return res;
    }

    // Matrix Multiplication
    inline Matrix4x4 operator*(const Matrix4x4& other) const
    {
        Matrix4x4 res;
        for (int r = 0; r < 4; r++)
        {
            for (int c = 0; c < 4; c++)
            {
                float val = 0;
                for (int k = 0; k < 4; k++)
                {
                    val += Get(r, k) * other.Get(k, c);
                }
                res.Set(r, c, val);
            }
        }
        return res;
    }

    // Transpose
    inline Matrix4x4 Transpose() const
    {
        Matrix4x4 res;
        for (int r = 0; r < 4; r++)
            for (int c = 0; c < 4; c++)
                res.Set(c, r, Get(r, c));
        return res;
    }
};

