#include "Vector4.h"


Vector4::Vector4()
{
    X = 0;
    Y = 0;
    Z = 0;
    W = 0;
}
Vector4::Vector4(float XYZW)
{
    X = XYZW;
    Y = XYZW;
    Z = XYZW;
    W = XYZW;
}
Vector4::Vector4(float X2, float Y2, float Z2, float W2)
{
    X = X2;
    Y = Y2;
    Z = Z2;
    W = W2;
}
Vector4 Vector4::operator+(Vector4 a)
{
    return Vector4(X + a.X, Y + a.Y, Z + a.Z, W + a.W);
}
Vector4 Vector4::operator-(Vector4 a)
{
    return Vector4(X - a.X, Y - a.Y, Z - a.Z, W - a.W);
}
Vector4 Vector4::operator*(float a)
{
    return Vector4(X * a, Y * a, Z * a, W * a);
}
Vector4 Vector4::operator/(float a)
{
    return Vector4(X / a, Y / a, Z / a, W / a);
}
Vector4 Vector4::Normalize(Vector4 a)
{
    float length = 1.0f / sqrtf(a.X * a.X + a.Y * a.Y + a.Z * a.Z + a.W * a.W);
    return Vector4(a.X * length, a.Y * length, a.Z * length, a.W * length);
}
float Vector4::Dot(Vector4 a, Vector4 b)
{
    return a.X * b.X + a.Y * b.Y + a.Z * b.Z + a.W * b.W;
}
