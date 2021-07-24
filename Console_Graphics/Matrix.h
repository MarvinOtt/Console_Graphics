#pragma once
#include "Vector4.h"
#include "Vector3.h"
struct Matrix
{
public:
    Vector4 rows[4];

    Matrix();
    Matrix(Vector4 vec1, Vector4 vec2, Vector4 vec3, Vector4 vec4);
    Vector3 TransposeVector3(Vector3 vec);
    Vector4 TransposeVector4(Vector4 vec);
    Matrix operator*(Matrix a);
    static Matrix CreateTranslation(Vector3 vec);
    static Matrix CreateRotationX(float angle);
    static Matrix CreateLookAt(Vector3 camerapos, Vector3 cameratarget, Vector3 upvector);
    static Matrix CreateFieldOfView(float angle, float aspectRatio, float nearPlane, float farPlane);
};

