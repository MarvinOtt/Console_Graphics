#pragma once
#include <math.h>
struct Vector4
{
public:
    float X, Y, Z, W;
    Vector4();
    Vector4(float XYZW);
    Vector4(float X2, float Y2, float Z2, float W2);
    Vector4 operator+(Vector4 a);
    Vector4 operator-(Vector4 a);
    Vector4 operator*(float a);
    Vector4 operator/(float a);
    static Vector4 Normalize(Vector4 a);
    static float Dot(Vector4 a, Vector4 b);
};

