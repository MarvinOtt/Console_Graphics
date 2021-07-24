#pragma once
struct Vector2
{
public:
    float X, Y;

    Vector2();
    Vector2(float XY);
    Vector2(float X2, float Y2);
    Vector2 operator+(Vector2 a);
    Vector2 operator-(Vector2 a);
    Vector2 operator*(float a);
    Vector2 operator/(float a);
    static float Dot(Vector2 a, Vector2 b);
    static float Cross(Vector2 a, Vector2 b);
};

