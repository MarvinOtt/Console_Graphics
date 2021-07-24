#pragma once
class RenderTarget
{
public:
    int Height, Width;
    unsigned char *DATA;
    float *DEPTH;
    
    RenderTarget();
    RenderTarget(int width, int height);
    void SetData(unsigned char* data, float* depth);
};

