#pragma once
#include "Vector4.h"

struct Vertex
{
public:
    Vector4 pos;
    float brighness;
};

class VertexBuffer
{
public:
    int Count;
    Vertex* vertexes;

    VertexBuffer();
    VertexBuffer(int count);
    void SetData(Vertex* verttices);
};

