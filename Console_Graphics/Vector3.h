#pragma once
#include <math.h>
struct Vector3
{
public:
    float X, Y, Z;

    Vector3();
    Vector3(float XYZ);
    Vector3(float X2, float Y2, float Z2);
    Vector3 operator+(Vector3 a);
    Vector3 operator-(Vector3 a);
    Vector3 operator*(float a);
    Vector3 operator/(float a);
    static Vector3 Normalize(Vector3 a);
    static float Dot(Vector3 a, Vector3 b);
    static Vector3 Cross(Vector3 a, Vector3 b);
};

