#pragma once

#define POINT_LIST 0
#define LINE_LIST 1
#define TRIANGLE_LIST 3

#include "VertexBuffer.h"
#include "RenderTarget.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Matrix.h"
#include "bse_console.h"
#include <stdio.h>

class GraphicsDevice
{
public:
    int RenderTargetCount;
    VertexBuffer* currentVertexBuffer;
    RenderTarget* currentRenderTarget;

public:
    unsigned char *asciibrighness;
    Vector4* L_clp1, *L_clp2, *L_clp3;
    float* L_str1, *L_str2, *L_str3;
    int L_length;
    Vector4* L2_clp1, *L2_clp2, *L2_clp3;
    float* L2_str1, *L2_str2, *L2_str3;
    int L2_length;
    float heightperpixel; // Height per pixel in NDC(Normalized Device Coordinates) Space
    float widthperpixel, minlength4drawing; // Width per pixel in NDC Space

    Vector2 vertexpos1, vertexpos2, vertexpos3, vertexposL1, vertexposL2;
    Vector4 cliped11, cliped12, cliped13, cliped21, cliped22, cliped23;
    float str11, str12, str13, str21, str22, str23;
    float vertexstrength1, vertexstrength2, vertexstrength3, vertexstrengthL1, vertexstrengthL2;
    float vertexdepth1, vertexdepth2, vertexdepth3, vertexdepthL1, vertexdepthL2;
    
public:
    GraphicsDevice();
    GraphicsDevice(unsigned char * ascii);
    void SetVertexBuffer(VertexBuffer* buffer);
    void SetRenderTarget(RenderTarget* buffer);
    void Clear(unsigned char Data);
    void DrawPrimitives(int mode, Matrix World, Matrix View, Matrix Projection);
    void DrawOnScreen();
    void DrawLine(RenderTarget *target, float startx, float starty, float endx, float endy);
    void DrawLine_L(RenderTarget *target, float startx, float starty, float endx, float endy);

private:
    static float Dot_Clipping(int index, Vector4 v);
    unsigned char GetClippingStates(Vector4 vec);
    unsigned int IsPosOnClipPlaneInFrustum(int clipplaneindex, Vector4 pos);
    unsigned char ClipLine(Vector4 inputpos1, Vector4 inputpos2, Vector4 *outputpos1, Vector4 *outputpos2, float *strength1, float *strength2);
    unsigned char ClipLineAtIndex(Vector4 inputpos1, Vector4 inputpos2, Vector4 *outputpos1, Vector4 *outputpos2, float *strength1, float *strength2, int index);
    Vector3 get_trianglevertexesstrength(Vector2 pos);
    void DrawHorizontalLine4Triangles(RenderTarget *target, float startx, float endx, float y);
    
    void DrawFlatTopTriangle(RenderTarget *target, Vector2 pos1, Vector2 pos2, Vector2 pos3);
    void DrawFlatBottomTriangle(RenderTarget *target, Vector2 pos1, Vector2 pos2, Vector2 pos3);
    void DrawFilledTriangle(RenderTarget *target, Vector4 worldviewprojectionpos1, Vector4 worldviewprojectionpos2, Vector4 worldviewprojectionpos3);
    void DrawWiredTriangle(RenderTarget *target, Vector4 worldviewprojectionpos1, Vector4 worldviewprojectionpos2, Vector4 worldviewprojectionpos3);
    int ClippAllTriangles(Vector4 worldviewprojectionpos1, Vector4 worldviewprojectionpos2, Vector4 worldviewprojectionpos3);
    void DrawClippedTriangles(RenderTarget *target);
    int ClipTriangle(Vector4 worldviewprojectionpos1, Vector4 worldviewprojectionpos2, Vector4 worldviewprojectionpos3, int side);

};

