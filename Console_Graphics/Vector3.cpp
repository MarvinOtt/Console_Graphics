#include "Vector3.h"


Vector3::Vector3()
{
    X = 0;
    Y = 0;
    Z = 0;
}

Vector3::Vector3(float XYZ)
{
    X = XYZ;
    Y = XYZ;
    Z = XYZ;
}
Vector3::Vector3(float X2, float Y2, float Z2)
{
    X = X2;
    Y = Y2;
    Z = Z2;
}
Vector3 Vector3::operator+(Vector3 a)
{
    return Vector3(X + a.X, Y + a.Y, Z + a.Z);
}
Vector3 Vector3::operator-(Vector3 a)
{
    return Vector3(X - a.X, Y - a.Y, Z - a.Z);
}
Vector3 Vector3::operator*(float a)
{
    return Vector3(X * a, Y * a, Z * a);
}
Vector3 Vector3::operator/(float a)
{
    return Vector3(X / a, Y / a, Z / a);
}
Vector3 Vector3::Normalize(Vector3 a)
{
    float length = 1.0f / sqrtf(a.X * a.X + a.Y * a.Y + a.Z * a.Z);
    return Vector3(a.X * length, a.Y * length, a.Z * length);


}
float Vector3::Dot(Vector3 a, Vector3 b)
{
    return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
}
Vector3 Vector3::Cross(Vector3 a, Vector3 b)
{
    Vector3 OUT;
    OUT.X = a.Y * b.Z - a.Z * b.Y;
    OUT.Y = a.Z * b.X - a.X * b.Z;
    OUT.Z = a.X * b.Y - a.Y * b.X;
    return OUT;
}

