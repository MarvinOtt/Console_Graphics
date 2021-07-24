#include "Matrix.h"

Matrix::Matrix()
{
    rows[0] = Vector4();
    rows[1] = Vector4();
    rows[2] = Vector4();
    rows[3] = Vector4();
}

Matrix::Matrix(Vector4 vec1, Vector4 vec2, Vector4 vec3, Vector4 vec4)
{
    rows[0] = vec1;
    rows[1] = vec2;
    rows[2] = vec3;
    rows[3] = vec4;
}
Vector3 Matrix::TransposeVector3(Vector3 vec)
{
    Vector3 output;

    output.X = rows[0].X * vec.X + rows[1].X * vec.Y + rows[2].X * vec.Z + rows[3].X;
    output.Y = rows[0].Y * vec.X + rows[1].Y * vec.Y + rows[2].Y * vec.Z + rows[3].Y;
    output.Z = rows[0].Z * vec.X + rows[1].Z * vec.Y + rows[2].Z * vec.Z + rows[3].Z;
    //float W = rows[0].W * vec.X + rows[1].W * vec.Y + rows[2].W * vec.Z + rows[3].W;
    return output;
}
Vector4 Matrix::TransposeVector4(Vector4 vec)
{
    Vector4 output;
    output.X = rows[0].X * vec.X + rows[1].X * vec.Y + rows[2].X * vec.Z + rows[3].X * vec.W;
    output.Y = rows[0].Y * vec.X + rows[1].Y * vec.Y + rows[2].Y * vec.Z + rows[3].Y * vec.W;
    output.Z = rows[0].Z * vec.X + rows[1].Z * vec.Y + rows[2].Z * vec.Z + rows[3].Z * vec.W;
    output.W = rows[0].W * vec.X + rows[1].W * vec.Y + rows[2].W * vec.Z + rows[3].W * vec.W;
    return output;
}
Matrix Matrix::CreateTranslation(Vector3 vec)
{
    return Matrix(Vector4(1, 0, 0, 0), Vector4(0, 1, 0, 0), Vector4(0, 0, 1, 0), Vector4(vec.X, vec.Y, vec.Z, 1));
}
Matrix Matrix::CreateRotationX(float angle)
{
    float value1 = cos(angle);
    float value2 = sin(angle);
    return Matrix(Vector4(1, 0, 0, 0), Vector4(0, value1, value2, 0), Vector4(0, -value2, value1, 0), Vector4(0, 0, 0, 1));
}
Matrix Matrix::operator*(Matrix m2)
{
    Matrix output;
    output.rows[0].X = rows[0].X * m2.rows[0].X + rows[0].Y * m2.rows[1].X + rows[0].Z * m2.rows[2].X + rows[0].W * m2.rows[3].X;
    output.rows[0].Y = rows[0].X * m2.rows[0].Y + rows[0].Y * m2.rows[1].Y + rows[0].Z * m2.rows[2].Y + rows[0].W * m2.rows[3].Y;
    output.rows[0].Z = rows[0].X * m2.rows[0].Z + rows[0].Y * m2.rows[1].Z + rows[0].Z * m2.rows[2].Z + rows[0].W * m2.rows[3].Z;
    output.rows[0].W = rows[0].X * m2.rows[0].W + rows[0].Y * m2.rows[1].W + rows[0].Z * m2.rows[2].W + rows[0].W * m2.rows[3].W;

    output.rows[1].X = rows[1].X * m2.rows[0].X + rows[1].Y * m2.rows[1].X + rows[1].Z * m2.rows[2].X + rows[1].W * m2.rows[3].X;
    output.rows[1].Y = rows[1].X * m2.rows[0].Y + rows[1].Y * m2.rows[1].Y + rows[1].Z * m2.rows[2].Y + rows[1].W * m2.rows[3].Y;
    output.rows[1].Z = rows[1].X * m2.rows[0].Z + rows[1].Y * m2.rows[1].Z + rows[1].Z * m2.rows[2].Z + rows[1].W * m2.rows[3].Z;
    output.rows[1].W = rows[1].X * m2.rows[0].W + rows[1].Y * m2.rows[1].W + rows[1].Z * m2.rows[2].W + rows[1].W * m2.rows[3].W;

    output.rows[2].X = rows[2].X * m2.rows[0].X + rows[2].Y * m2.rows[1].X + rows[2].Z * m2.rows[2].X + rows[2].W * m2.rows[3].X;
    output.rows[2].Y = rows[2].X * m2.rows[0].Y + rows[2].Y * m2.rows[1].Y + rows[2].Z * m2.rows[2].Y + rows[2].W * m2.rows[3].Y;
    output.rows[2].Z = rows[2].X * m2.rows[0].Z + rows[2].Y * m2.rows[1].Z + rows[2].Z * m2.rows[2].Z + rows[2].W * m2.rows[3].Z;
    output.rows[2].W = rows[2].X * m2.rows[0].W + rows[2].Y * m2.rows[1].W + rows[2].Z * m2.rows[2].W + rows[2].W * m2.rows[3].W;

    output.rows[3].X = rows[3].X * m2.rows[0].X + rows[3].Y * m2.rows[1].X + rows[3].Z * m2.rows[2].X + rows[3].W * m2.rows[3].X;
    output.rows[3].Y = rows[3].X * m2.rows[0].Y + rows[3].Y * m2.rows[1].Y + rows[3].Z * m2.rows[2].Y + rows[3].W * m2.rows[3].Y;
    output.rows[3].Z = rows[3].X * m2.rows[0].Z + rows[3].Y * m2.rows[1].Z + rows[3].Z * m2.rows[2].Z + rows[3].W * m2.rows[3].Z;
    output.rows[3].W = rows[3].X * m2.rows[0].W + rows[3].Y * m2.rows[1].W + rows[3].Z * m2.rows[2].W + rows[3].W * m2.rows[3].W;
    return output;
}
Matrix Matrix::CreateLookAt(Vector3 camerapos, Vector3 cameratarget, Vector3 upvector)
{
    Vector3 forward = Vector3::Normalize(camerapos - cameratarget);
    Vector3 right = Vector3::Normalize(Vector3::Cross(upvector, forward));
    Vector3 up = Vector3::Cross(forward, right);
    /*Matrix_Init(&output,
                Vector4_Init(right.X, right.Y, right.Z, -Vector3_Dot(right, camerapos)),
                Vector4_Init(up.X, up.Y, up.Z, -Vector3_Dot(up, camerapos)),
                Vector4_Init(forward.X, forward.Y, forward.Z, -Vector3_Dot(forward, camerapos)),
                Vector4_Init(0, 0, 0, 1));*/
    return Matrix(Vector4(right.X, up.X, forward.X, 0), Vector4(right.Y, up.Y, forward.Y, 0), Vector4(right.Z, up.Z, forward.Z, 0), Vector4(-Vector3::Dot(right, camerapos), -Vector3::Dot(up, camerapos), -Vector3::Dot(forward, camerapos), 1));
}
Matrix Matrix::CreateFieldOfView(float angle, float AspectRatio, float nearPlane, float farPlane)
{
    float scale = 1 / tan(angle * 0.5f);
    return Matrix(Vector4(scale / AspectRatio, 0, 0, 0), Vector4(0, scale, 0, 0), Vector4(0, 0, (farPlane / (nearPlane - farPlane)), -1), Vector4(0, 0, ((farPlane * nearPlane) / (nearPlane - farPlane)), 0));
}
