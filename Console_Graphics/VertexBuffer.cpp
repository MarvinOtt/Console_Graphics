#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
{
    Count = 0;
}

VertexBuffer::VertexBuffer(int count)
{
    Count = count;
}

void VertexBuffer::SetData(Vertex* vertices)
{
    vertexes = vertices;
}

