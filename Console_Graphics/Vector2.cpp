#include "Vector2.h"

Vector2::Vector2()
{
    X = 0;
    Y = 0;
}
Vector2::Vector2(float XY)
{
    X = XY;
    Y = XY;
}
Vector2::Vector2(float X2, float Y2)
{
    X = X2;
    Y = Y2;
}
Vector2 Vector2::operator+(Vector2 a)
{
    return Vector2(X + a.X, Y + a.Y);
}
Vector2 Vector2::operator-(Vector2 a)
{
    return Vector2(X - a.X, Y - a.Y);
}
Vector2 Vector2::operator*(float a)
{
    return Vector2(X * a, Y * a);
}
Vector2 Vector2::operator/(float a)
{
    return Vector2(X / a, Y / a);
}
float Vector2::Dot(Vector2 a, Vector2 b)
{
    return a.X * b.X + a.Y * b.Y;
}
float Vector2::Cross(Vector2 a, Vector2 b)
{
    return a.X * b.Y - a.Y * b.X;
}
