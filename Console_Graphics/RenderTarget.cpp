#include "RenderTarget.h"

RenderTarget::RenderTarget()
{

}
RenderTarget::RenderTarget(int Width2, int Height2)
{
    Width = Width2;
    Height = Height2;
}

void RenderTarget::SetData(unsigned char * DATA2, float *DEPTH2)
{
    DATA = DATA2;
    DEPTH = DEPTH2;
}

