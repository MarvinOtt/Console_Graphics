#include "GraphicsDevice.h"
GraphicsDevice::GraphicsDevice()
{

}
GraphicsDevice::GraphicsDevice(unsigned char* ascii)
{
    asciibrighness = ascii;
    RenderTargetCount = 0;
    L_clp1 = new Vector4[30];// (Vector4*)malloc(30 * sizeof(Vector4));
    L_clp2 = new Vector4[30];
    L_clp3 = new Vector4[30];
    L_str1 = new float[30];
    L_str2 = new float[30];
    L_str3 = new float[30];
    L2_clp1 = new Vector4[30];
    L2_clp2 = new Vector4[30];
    L2_clp3 = new Vector4[30];
    L2_str1 = new float[30];
    L2_str2 = new float[30];
    L2_str3 = new float[30];
    L_length = L2_length = 0;
}

void GraphicsDevice::SetVertexBuffer(VertexBuffer *buffer)
{
    currentVertexBuffer = buffer;
}

void GraphicsDevice::SetRenderTarget(RenderTarget *target)
{
    currentRenderTarget = target;
    heightperpixel = 2 / (float)target->Height;
    widthperpixel = 2 / ((float)target->Width);
    minlength4drawing = min(heightperpixel, widthperpixel) * 0.999f;
}

void GraphicsDevice::Clear(unsigned char Data)
{
    int x, y;
    for (x = 0; x < currentRenderTarget->Width; ++x)
    {
        for (y = 0; y < currentRenderTarget->Height; ++y)
        {
            currentRenderTarget->DATA[x + y * currentRenderTarget->Width] = Data;
            currentRenderTarget->DEPTH[x + y * currentRenderTarget->Width] = 999999.0f;
        }
    }
}

float GraphicsDevice::Dot_Clipping(int index2, Vector4 v)
{
    switch (index2)
    {
    case 0:
        return v.X + v.W;    /// LEFT
    case 1:
        return -v.X + v.W;   /// RIGHT
    case 2:
        return v.Y + v.W;    /// UP
    case 3:
        return -v.Y + v.W;   /// DOWN
    case 4:
        return -v.Z + v.W;   /// FRONT
    case 5:
        return v.Z;          /// BACK
    }
    return 0;
}

unsigned char GraphicsDevice::GetClippingStates(Vector4 vec)
{
    int i;
    unsigned char output = 0;
    for (i = 0; i < 6; ++i)
    {
        if (Dot_Clipping(i, vec) < 0) output |= (1 << i);
    }
    return output;
}

unsigned int GraphicsDevice::IsPosOnClipPlaneInFrustum(int clipplaneindex, Vector4 pos)
{
    int i;
    for (i = 0; i < 6; ++i)
    {
        if (i != clipplaneindex)
            if (Dot_Clipping(i, pos) < 0)
                return 0;
    }
    return 1;
}

unsigned char GraphicsDevice::ClipLine(Vector4 inputpos1, Vector4 inputpos2, Vector4 *outputpos1, Vector4 *outputpos2, float *strength1, float *strength2)
{
    Vector4 OUT1, OUT2;
    unsigned char inputpos1_states = GetClippingStates(inputpos1);
    unsigned char inputpos2_states = GetClippingStates(inputpos2);
    int numberofintersections = 0;
    Vector4 newpositions[2];
    float newstrengths[2];
    if ((inputpos1_states & inputpos2_states) == 0)
    {
        unsigned char mask = inputpos1_states | inputpos2_states;
        if (mask == 0)
        {
            *outputpos1 = inputpos1;
            *outputpos2 = inputpos2;
            return 1;
        }
        else
        {
            int i;
            for (i = 0; i < 6; i++)
            {
                if (mask & (1 << i)) /// The Line is intersecting this Clipping Plane
                {
                    float a = Dot_Clipping(i, inputpos1);
                    float b = Dot_Clipping(i, inputpos2);
                    float alpha1 = fabs(a) / (fabs(a) + fabs(b));
                    float alpha2 = 1 - alpha1;
                    Vector4 newpos = inputpos1 * alpha2 + inputpos2 * alpha1;
                    if (IsPosOnClipPlaneInFrustum(i, newpos)) /// Clipped Position is in the Frustum
                    {
                        newpositions[numberofintersections] = newpos;
                        newstrengths[numberofintersections] = (*strength1) * alpha2 + (*strength2) * alpha1;
                        numberofintersections++;
                    }
                }
            }
        }
    }
    else
        return 0;
    if (numberofintersections == 2)
    {
        *outputpos1 = newpositions[0];
        *outputpos2 = newpositions[1];
        *strength1 = newstrengths[0];
        *strength2 = newstrengths[1];
        return 1;
    }
    else if (numberofintersections == 1)
    {
        if (inputpos1_states == 0)
        {
            *outputpos1 = inputpos1;
            *outputpos2 = newpositions[0];
            *strength2 = newstrengths[0];
            return 1;
        }
        else
        {
            *outputpos1 = newpositions[0];
            *outputpos2 = inputpos2;
            *strength1 = newstrengths[0];
            return 1;
        }
    }
    return 0;
}

unsigned char GraphicsDevice::ClipLineAtIndex(Vector4 inputpos1, Vector4 inputpos2, Vector4 *outputpos1, Vector4 *outputpos2, float *strength1, float *strength2, int index)
{
    float a = Dot_Clipping(index, inputpos1);
    float b = Dot_Clipping(index, inputpos2);
    Vector4 newpos;
    if (a > 0)
    {
        float alpha1 = fabs(a) / (fabs(a) + fabs(b));
        newpos = inputpos1 +  (inputpos2 - inputpos1) * alpha1;
        *outputpos1 = inputpos1;
        *outputpos2 = newpos;
        *strength2 = (*strength1) * (1.0f - alpha1) + (*strength2) * alpha1;
        return 1;
    }
    else
    {
        float alpha1 = fabs(b) / (fabs(a) + fabs(b));
        newpos = inputpos2 + (inputpos1 - inputpos2) * alpha1;
        *outputpos1 = newpos;
        *outputpos2 = inputpos2;
        *strength1 = (*strength2) * (1.0f - alpha1) + (*strength1) * alpha1;
        return 1;
    }

    return 0;
}

Vector3 GraphicsDevice::get_trianglevertexesstrength(Vector2 pos)
{
    Vector3 output;
    float triangleareax2 = fabs(Vector2::Cross(vertexpos2 - vertexpos1, vertexpos3 - vertexpos1));

    output.X = fabs(Vector2::Cross(vertexpos3 - vertexpos2, pos - vertexpos2)) / triangleareax2;
    output.Y = fabs(Vector2::Cross(vertexpos3 - vertexpos1, pos - vertexpos1)) / triangleareax2;
    output.Z = fabs(Vector2::Cross(vertexpos2 - vertexpos1, pos - vertexpos1)) / triangleareax2;
    return output;
}

void GraphicsDevice::DrawHorizontalLine4Triangles(RenderTarget *target, float startx, float endx, float y)
{
    const float direction = endx - startx;
    const float length = fabs(direction);
    const float onedivlength = 1.0f / length;
    const int height = target->Height;
    const int width = target->Width;
    const int height_index = (y + 1) * 0.5f * height;
    const int curpixelY = height_index * target->Width;
    unsigned char* curdata = target->DATA;
    float* curdepth = target->DEPTH;
    for (float currentlength = 0; currentlength < length; currentlength += minlength4drawing)
    {
        float curx = startx + direction * (currentlength  * onedivlength);
        int width_index = (curx + 1) * 0.5f * width;
        Vector3 strengths = get_trianglevertexesstrength(Vector2(curx, y));
        if (width_index >= 0 && width_index < width && height_index >= 0 && height_index < height)
        {
            // Calculating Depth and strength for the specific pixel
            float finaldepth = 1.0f / (strengths.X * vertexdepth1 + strengths.Y * vertexdepth2 + strengths.Z * vertexdepth3);
            float finalbrighness = finaldepth * (vertexstrength1 * vertexdepth1 * strengths.X + vertexstrength2 * vertexdepth2 * strengths.Y + vertexstrength3 * vertexdepth3 * strengths.Z);
            if (finaldepth < curdepth[curpixelY + width_index]) // Object is not behind other objects
            {
                curdata[curpixelY + width_index] = (unsigned char)(finalbrighness * 255);
                curdepth[curpixelY + width_index] = finaldepth;
            }
        }
    }
}

void GraphicsDevice::DrawLine(RenderTarget *target, float startx, float starty, float endx, float endy)
{
    int width_index, height_index;
    float currentlength;
    Vector2 difvector2 = Vector2(endx - startx, endy - starty);
    float length = sqrtf(difvector2.X * difvector2.X + difvector2.Y * difvector2.Y);
    float lengthinv = 1.0f / length;
    unsigned char* curdata = target->DATA;
    float* curdepth = target->DEPTH;
    for (currentlength = 0; currentlength < length; currentlength += minlength4drawing)
    {
        float curx = startx + difvector2.X * (currentlength * lengthinv);
        float cury = starty + difvector2.Y * (currentlength * lengthinv);
        width_index = (curx + 1) * 0.5f * target->Width;
        height_index = (cury + 1) * 0.5f * target->Height;
        Vector3 strengths = get_trianglevertexesstrength(Vector2(curx, cury));
        if (width_index >= 0 && width_index < target->Width && height_index >= 0 && height_index < target->Height)
        {
            float finaldepth = 1 / (strengths.X * vertexdepth1 + strengths.Y * vertexdepth2 + strengths.Z * vertexdepth3);
            float finalbrighness = finaldepth * (vertexstrength1 * vertexdepth1 * strengths.X + vertexstrength2 * vertexdepth2 * strengths.Y + vertexstrength3 * vertexdepth3 * strengths.Z);
            if (finaldepth < target->DEPTH[width_index + height_index * target->Width])
            {
                curdata[width_index + height_index * target->Width] = (unsigned char)(finalbrighness * 255);//asciibrighness[(int)(finalbrighness * 999)];
                curdepth[width_index + height_index * target->Width] = finaldepth;
            }
        }
    }
}

void GraphicsDevice::DrawLine_L(RenderTarget *target, float startx, float starty, float endx, float endy)
{
    int width_index, height_index;
    float currentlength;
    Vector2 difvector2 = Vector2(endx - startx, endy - starty);
    float length = sqrtf(difvector2.X * difvector2.X + difvector2.Y * difvector2.Y);
    float lengthinv = 1.0f / length;
    unsigned char* curdata = target->DATA;
    float* curdepth = target->DEPTH;
    for (currentlength = 0; currentlength < length; currentlength += minlength4drawing)
    {
        float curx = startx + difvector2.X * (currentlength * lengthinv);
        float cury = starty + difvector2.Y * (currentlength * lengthinv);
        width_index = (curx + 1) * 0.5f * target->Width;
        height_index = (cury + 1) * 0.5f * target->Height;
        Vector2 strengths;
        strengths.X = 1 - currentlength / length;
        strengths.Y = 1 - strengths.X;
        if (width_index >= 0 && width_index < target->Width && height_index >= 0 && height_index < target->Height)
        {
            float finaldepth = 1 / (strengths.X * vertexdepth1 + strengths.Y * vertexdepth2);
            float finalbrighness = finaldepth * (vertexstrength1 * vertexdepth1 * strengths.X + vertexstrength2 * vertexdepth2 * strengths.Y);
            if (finaldepth < target->DEPTH[width_index + height_index * target->Width])
            {
                curdata[width_index + height_index * target->Width] = (unsigned char)(finalbrighness * 255);//asciibrighness[(int)(finalbrighness * 999)];
                curdepth[width_index + height_index * target->Width] = finaldepth;
            }
        }
    }
}

void GraphicsDevice::DrawFlatTopTriangle(RenderTarget *target, Vector2 pos1, Vector2 pos2, Vector2 pos3)
{
    float heightperpixel = 2 / (float)target->Height;
    float widthperpixel = 1 / (float)target->Width;
    float inversk1 = ((pos2.X - pos1.X) / (pos2.Y - pos1.Y)) * widthperpixel;
    float inversk2 = ((pos3.X - pos1.X) / (pos3.Y - pos1.Y)) * widthperpixel;
    float ypos, curx1 = pos1.X, curx2 = pos1.X;
    for (ypos = pos1.Y; ypos > pos2.Y; ypos -= widthperpixel)
    {
        DrawHorizontalLine4Triangles(target, curx1, curx2, ypos);
        curx1 -= inversk1;
        curx2 -= inversk2;
    }
    DrawLine(target, pos1.X, pos1.Y, pos2.X, pos2.Y);
    DrawLine(target, pos1.X, pos1.Y, pos3.X, pos3.Y);
    DrawLine(target, pos2.X, pos2.Y, pos3.X, pos3.Y);
}

void GraphicsDevice::DrawFlatBottomTriangle(RenderTarget *target, Vector2 pos1, Vector2 pos2, Vector2 pos3)
{
    float heightperpixel = 2 / (float)target->Height;
    float widthperpixel = 2 / (float)target->Width;
    float inversk1 = ((pos2.X - pos1.X) / (pos2.Y - pos1.Y)) * widthperpixel;
    float inversk2 = ((pos3.X - pos1.X) / (pos3.Y - pos1.Y)) * widthperpixel;
    float ypos, curx1 = pos1.X, curx2 = pos1.X;
    for (ypos = pos1.Y; ypos < pos2.Y; ypos += widthperpixel)
    {
        DrawHorizontalLine4Triangles(target, curx1, curx2, ypos);
        curx1 += inversk1;
        curx2 += inversk2;
    }
    DrawLine(target, pos1.X, pos1.Y, pos2.X, pos2.Y);
    DrawLine(target, pos1.X, pos1.Y, pos3.X, pos3.Y);
    DrawLine(target, pos2.X, pos2.Y, pos3.X, pos3.Y);
}

void GraphicsDevice::DrawFilledTriangle(RenderTarget *target, Vector4 worldviewprojectionpos1, Vector4 worldviewprojectionpos2, Vector4 worldviewprojectionpos3)
{
    Vector2 toppos, bottompos, midlepos;
    int topid, bottomid;
    if (worldviewprojectionpos1.Y <= worldviewprojectionpos2.Y && worldviewprojectionpos1.Y <= worldviewprojectionpos3.Y)
    {
        topid = 1;
        toppos = Vector2(worldviewprojectionpos1.X, worldviewprojectionpos1.Y);
    }
    else if (worldviewprojectionpos2.Y <= worldviewprojectionpos1.Y && worldviewprojectionpos2.Y <= worldviewprojectionpos3.Y)
    {
        topid = 2;
        toppos = Vector2(worldviewprojectionpos2.X, worldviewprojectionpos2.Y);
    }
    else
    {
        topid = 3;
        toppos = Vector2(worldviewprojectionpos3.X, worldviewprojectionpos3.Y);
    }

    if (worldviewprojectionpos1.Y >= worldviewprojectionpos2.Y && worldviewprojectionpos1.Y >= worldviewprojectionpos3.Y && topid != 1)
    {
        bottomid = 1;
        bottompos = Vector2(worldviewprojectionpos1.X, worldviewprojectionpos1.Y);
    }
    else if (worldviewprojectionpos2.Y >= worldviewprojectionpos1.Y && worldviewprojectionpos2.Y >= worldviewprojectionpos3.Y && topid != 2)
    {
        bottomid = 2;
        bottompos = Vector2(worldviewprojectionpos2.X, worldviewprojectionpos2.Y);
    }
    else
    {
        bottomid = 3;
        bottompos = Vector2(worldviewprojectionpos3.X, worldviewprojectionpos3.Y);
    }

    if (bottomid != 1 && topid != 1)
        midlepos = Vector2(worldviewprojectionpos1.X, worldviewprojectionpos1.Y);
    else if (bottomid != 2 && topid != 2)
        midlepos = Vector2(worldviewprojectionpos2.X, worldviewprojectionpos2.Y);
    else
        midlepos = Vector2(worldviewprojectionpos3.X, worldviewprojectionpos3.Y);

    if (bottompos.Y == midlepos.Y)
        DrawFlatBottomTriangle(target, toppos, bottompos, midlepos);

    if (toppos.Y == midlepos.Y)
        DrawFlatTopTriangle(target, bottompos, midlepos, toppos);

    float pos4_X = toppos.X + ((midlepos.Y - toppos.Y) / (bottompos.Y - toppos.Y)) * (bottompos.X - toppos.X);



    DrawFlatBottomTriangle(target, toppos, Vector2(pos4_X, midlepos.Y), midlepos);
    DrawFlatTopTriangle(target, bottompos, midlepos, Vector2(pos4_X, midlepos.Y));
}

void GraphicsDevice::DrawWiredTriangle(RenderTarget *target, Vector4 worldviewprojectionpos1, Vector4 worldviewprojectionpos2, Vector4 worldviewprojectionpos3)
{
    DrawLine(target, worldviewprojectionpos1.X, worldviewprojectionpos1.Y, worldviewprojectionpos2.X, worldviewprojectionpos2.Y);
    DrawLine(target, worldviewprojectionpos1.X, worldviewprojectionpos1.Y, worldviewprojectionpos3.X, worldviewprojectionpos3.Y);
    DrawLine(target, worldviewprojectionpos2.X, worldviewprojectionpos2.Y, worldviewprojectionpos3.X, worldviewprojectionpos3.Y);
}

void swapArray(void **a, void **b)
{
    void *p = *a;
    *a = *b;
    *b = p;
}

int GraphicsDevice::ClippAllTriangles(Vector4 worldviewprojectionpos1, Vector4 worldviewprojectionpos2, Vector4 worldviewprojectionpos3)
{
    L2_clp1[0] = worldviewprojectionpos1;
    L2_clp2[0] = worldviewprojectionpos2;
    L2_clp3[0] = worldviewprojectionpos3;
    L2_str1[0] = vertexstrength1;
    L2_str2[0] = vertexstrength2;
    L2_str3[0] = vertexstrength3;
    L2_length = 1;

    for (int i = 0; i < 6; ++i)
    {
        if (L2_length == 0)
            break;
        L_length = 0;
        for (int j = 0; j < L2_length; ++j)
        {
            vertexstrength1 = L2_str1[j];
            vertexstrength2 = L2_str2[j];
            vertexstrength3 = L2_str3[j];
            int length = ClipTriangle(L2_clp1[j], L2_clp2[j], L2_clp3[j], i);

            if (length == 1)
            {
                L_clp1[L_length] = cliped11;
                L_clp2[L_length] = cliped12;
                L_clp3[L_length] = cliped13;
                L_str1[L_length] = str11;
                L_str2[L_length] = str12;
                L_str3[L_length] = str13;
            }
            else if (length == 2)
            {
                L_clp1[L_length] = cliped11;
                L_clp2[L_length] = cliped12;
                L_clp3[L_length] = cliped13;
                L_str1[L_length] = str11;
                L_str2[L_length] = str12;
                L_str3[L_length] = str13;

                L_clp1[L_length + 1] = cliped21;
                L_clp2[L_length + 1] = cliped22;
                L_clp3[L_length + 1] = cliped23;
                L_str1[L_length + 1] = str21;
                L_str2[L_length + 1] = str22;
                L_str3[L_length + 1] = str23;
            }

            L_length += length;
        }
        //Swop L and L2
        swapArray((void**)&L_clp1, (void**)&L2_clp1);
        swapArray((void**)&L_clp2, (void**)&L2_clp2);
        swapArray((void**)&L_clp3, (void**)&L2_clp3);
        swapArray((void**)&L_str1, (void**)&L2_str1);
        swapArray((void**)&L_str2, (void**)&L2_str2);
        swapArray((void**)&L_str3, (void**)&L2_str3);
        L2_length = L_length;
    }
    return L2_length;
}

void GraphicsDevice::DrawClippedTriangles(RenderTarget *target)
{

    int i;
    for (i = 0; i < L2_length; ++i)
    {
        vertexstrength1 = L2_str1[i];
        vertexstrength2 = L2_str2[i];
        vertexstrength3 = L2_str3[i];


        float invers_w = 1 / L2_clp1[i].W;

        L2_clp1[i].X *= invers_w;
        L2_clp1[i].Y *= invers_w;
        L2_clp1[i].Z *= invers_w;
        invers_w = 1 / L2_clp2[i].W;

        L2_clp2[i].X *= invers_w;
        L2_clp2[i].Y *= invers_w;
        L2_clp2[i].Z *= invers_w;
        invers_w = 1 / L2_clp3[i].W;

        L2_clp3[i].X *= invers_w;
        L2_clp3[i].Y *= invers_w;
        L2_clp3[i].Z *= invers_w;



        vertexdepth1 = 1.0f / L2_clp1[i].W;
        vertexdepth2 = 1.0f / L2_clp2[i].W;
        vertexdepth3 = 1.0f / L2_clp3[i].W;

        vertexpos1 = Vector2(L2_clp1[i].X, L2_clp1[i].Y);
        vertexpos2 = Vector2(L2_clp2[i].X, L2_clp2[i].Y);
        vertexpos3 = Vector2(L2_clp3[i].X, L2_clp3[i].Y);

       
        DrawFilledTriangle(target, L2_clp1[i], L2_clp2[i], L2_clp3[i]);
        //DrawWiredTriangle(target, L2_clp1[i], L2_clp2[i], L2_clp3[i]);
    }
}

int GraphicsDevice::ClipTriangle(Vector4 worldviewprojectionpos1, Vector4 worldviewprojectionpos2, Vector4 worldviewprojectionpos3, int side)
{
    int state1 = Dot_Clipping(side, worldviewprojectionpos1) < 0;
    int state2 = Dot_Clipping(side, worldviewprojectionpos2) < 0;
    int state3 = Dot_Clipping(side, worldviewprojectionpos3) < 0;
    int sum = state1 + state2 + state3;
    if (state1 == 0 && state2 == 0 && state3 == 0)
    {
        cliped11 = worldviewprojectionpos1;
        cliped12 = worldviewprojectionpos2;
        cliped13 = worldviewprojectionpos3;
        str11 = vertexstrength1;
        str12 = vertexstrength2;
        str13 = vertexstrength3;
        return 1;
    }
    else
    {
        if (sum == 1)
        {
            Vector4 cl1, cl2;
            float s1, s2;
            if (state1 > 0)
            {
                s1 = vertexstrength1;
                s2 = s1;
                ClipLineAtIndex(worldviewprojectionpos1, worldviewprojectionpos2, &cl1, &worldviewprojectionpos2, &s1, &vertexstrength2, side);
                ClipLineAtIndex(worldviewprojectionpos1, worldviewprojectionpos3, &cl2, &worldviewprojectionpos3, &s2, &vertexstrength3, side);
                cliped12 = worldviewprojectionpos2;
                cliped13 = worldviewprojectionpos3;
                cliped22 = worldviewprojectionpos3;
                str12 = vertexstrength2;
                str13 = vertexstrength3;
                str22 = vertexstrength3;
            }
            else if (state2 > 0)
            {
                s1 = vertexstrength2;
                s2 = s1;
                ClipLineAtIndex(worldviewprojectionpos2, worldviewprojectionpos3, &cl1, &worldviewprojectionpos3, &s1, &vertexstrength3, side);
                ClipLineAtIndex(worldviewprojectionpos2, worldviewprojectionpos1, &cl2, &worldviewprojectionpos1, &s2, &vertexstrength1, side);
                cliped12 = worldviewprojectionpos3;
                cliped13 = worldviewprojectionpos1;
                cliped22 = worldviewprojectionpos1;
                str12 = vertexstrength3;
                str13 = vertexstrength1;
                str22 = vertexstrength1;
            }
            else
            {
                s1 = vertexstrength3;
                s2 = s1;
                ClipLineAtIndex(worldviewprojectionpos3, worldviewprojectionpos1, &cl1, &worldviewprojectionpos1, &s1, &vertexstrength1, side);
                ClipLineAtIndex(worldviewprojectionpos3, worldviewprojectionpos2, &cl2, &worldviewprojectionpos2, &s2, &vertexstrength2, side);
                cliped12 = worldviewprojectionpos1;
                cliped13 = worldviewprojectionpos2;
                cliped22 = worldviewprojectionpos2;
                str12 = vertexstrength1;
                str13 = vertexstrength2;
                str22 = vertexstrength2;
            }
            cliped11 = cl1;
            cliped21 = cl1;
            cliped23 = cl2;
            str11 = s1;
            str21 = s1;
            str23 = s2;
            return 2;
        }
        else if (sum == 2)
        {
            if (state1 == 0)
            {
                ClipLineAtIndex(worldviewprojectionpos1, worldviewprojectionpos3, &worldviewprojectionpos1, &worldviewprojectionpos3, &vertexstrength1, &vertexstrength3, side);
                ClipLineAtIndex(worldviewprojectionpos1, worldviewprojectionpos2, &worldviewprojectionpos1, &worldviewprojectionpos2, &vertexstrength1, &vertexstrength2, side);
            }
            else if (state2 == 0)
            {
                ClipLineAtIndex(worldviewprojectionpos2, worldviewprojectionpos1, &worldviewprojectionpos2, &worldviewprojectionpos1, &vertexstrength2, &vertexstrength1, side);
                ClipLineAtIndex(worldviewprojectionpos2, worldviewprojectionpos3, &worldviewprojectionpos2, &worldviewprojectionpos3, &vertexstrength2, &vertexstrength3, side);
            }
            else if (state3 == 0)
            {
                ClipLineAtIndex(worldviewprojectionpos3, worldviewprojectionpos2, &worldviewprojectionpos3, &worldviewprojectionpos2, &vertexstrength3, &vertexstrength2, side);
                ClipLineAtIndex(worldviewprojectionpos3, worldviewprojectionpos1, &worldviewprojectionpos3, &worldviewprojectionpos1, &vertexstrength3, &vertexstrength1, side);
            }
            cliped11 = worldviewprojectionpos1;
            cliped12 = worldviewprojectionpos2;
            cliped13 = worldviewprojectionpos3;
            str11 = vertexstrength1;
            str12 = vertexstrength2;
            str13 = vertexstrength3;
            return 1;
        }
        else
            return 0;
    }
}

void GraphicsDevice::DrawPrimitives(int mode, Matrix World, Matrix View, Matrix Projection)
{
    int i;
    Matrix worldviewmatrix = World * View;
    Matrix worldviewprojectionmatrix = worldviewmatrix * Projection;
    if (mode == 0)
    {
#ifndef DRAWING_POINT_LIST
        for (i = 0; i < currentVertexBuffer->Count; ++i)
        {
            vertexstrength1 = currentVertexBuffer->vertexes[i].brighness;
            Vector4 Transformedpos = worldviewprojectionmatrix.TransposeVector4(currentVertexBuffer->vertexes[i].pos);

            unsigned char output = GetClippingStates(Transformedpos);
            if (output == 0) /// Vertex is in Viewing Frustum
            {
                /// Dividing with Depth for 3D Effect
                float invers_w = 1 / Transformedpos.W;
                Transformedpos.X *= invers_w;
                Transformedpos.Y *= invers_w;
                Transformedpos.Z *= invers_w;

                /// Getting Screen Coordinates and set the Color
                int width_index, height_index;
                width_index = (Transformedpos.X + 1) * 0.5f * currentRenderTarget->Width;
                height_index = (Transformedpos.Y + 1) * 0.5f * currentRenderTarget->Height;

                float finaldepth = Transformedpos.W;
                float finalbrighness = currentVertexBuffer->vertexes[i].brighness;
                if (finaldepth < currentRenderTarget->DEPTH[width_index + height_index * currentRenderTarget->Width])
                {
                    currentRenderTarget->DATA[width_index + height_index * currentRenderTarget->Width] = (int)(finalbrighness * 255);//asciibrighness[(int)(finalbrighness * 999)];
                    currentRenderTarget->DEPTH[width_index + height_index * currentRenderTarget->Width] = finaldepth;
                }
            }

        }
#endif
    }
    else if (mode == 1)
    {
#ifndef DRAWING_LINE_LIST
        for (i = 0; i < currentVertexBuffer->Count - 1; i += 2)
        {
            vertexstrength1 = currentVertexBuffer->vertexes[i].brighness;
            vertexstrength2 = currentVertexBuffer->vertexes[i + 1].brighness;



            Vector4 worldviewprojectionpos1, worldviewprojectionpos2;
            worldviewprojectionpos1 = worldviewprojectionmatrix.TransposeVector4(currentVertexBuffer->vertexes[i].pos);
            worldviewprojectionpos2 = worldviewprojectionmatrix.TransposeVector4(currentVertexBuffer->vertexes[i + 1].pos);
            unsigned char IsinFrustum = ClipLine(worldviewprojectionpos1, worldviewprojectionpos2, &worldviewprojectionpos1, &worldviewprojectionpos2, &vertexstrength1, &vertexstrength2);

            if (IsinFrustum)
            {
                /// Dividing with Depth for 3D Effect
                float invers_w = 1 / worldviewprojectionpos1.W;
                worldviewprojectionpos1.X *= invers_w;
                worldviewprojectionpos1.Y *= invers_w;
                worldviewprojectionpos1.Z *= invers_w;
                invers_w = 1 / worldviewprojectionpos2.W;
                worldviewprojectionpos2.X *= invers_w;
                worldviewprojectionpos2.Y *= invers_w;
                worldviewprojectionpos2.Z *= invers_w;

                vertexdepth1 = 1.0f / worldviewprojectionpos1.W;
                vertexdepth2 = 1.0f / worldviewprojectionpos2.W;

                DrawLine_L(currentRenderTarget, worldviewprojectionpos1.X, worldviewprojectionpos1.Y, worldviewprojectionpos2.X, worldviewprojectionpos2.Y);
            }
        }
#endif
    }
    else if (mode == 3)
    {
#ifndef DRAWING_TRIANGLE_LIST
        for (i = 0; i < currentVertexBuffer->Count - 2; i += 3)
        {
            vertexstrength1 = currentVertexBuffer->vertexes[i].brighness;
            vertexstrength2 = currentVertexBuffer->vertexes[i + 1].brighness;
            vertexstrength3 = currentVertexBuffer->vertexes[i + 2].brighness;

            Vector4 worldviewprojectionpos1, worldviewprojectionpos2, worldviewprojectionpos3;
            worldviewprojectionpos1 = worldviewprojectionmatrix.TransposeVector4(currentVertexBuffer->vertexes[i].pos);
            worldviewprojectionpos2 = worldviewprojectionmatrix.TransposeVector4(currentVertexBuffer->vertexes[i + 1].pos);
            worldviewprojectionpos3 = worldviewprojectionmatrix.TransposeVector4(currentVertexBuffer->vertexes[i + 2].pos);

            //char str[50];
            //sprintf_s(str, "%f I %f", worldviewprojectionpos1.X + worldviewprojectionpos1.W, worldviewprojectionpos1.W);

            //wchar_t wtext[50];
            //size_t outSize;
            //mbstowcs_s(&outSize, wtext, strlen(str) + 1, str, strlen(str));//Plus null
            //LPWSTR ptr = wtext;
            //SetConsoleTitle(ptr);

            int remainingTris = ClippAllTriangles(worldviewprojectionpos1, worldviewprojectionpos2, worldviewprojectionpos3);
            if(remainingTris > 0)
                DrawClippedTriangles(currentRenderTarget);
        }
#endif
    }
}

void GraphicsDevice::DrawOnScreen()
{
    for (int x = 0; x < currentRenderTarget->Width; ++x)
    {
        for (int y = 0; y < currentRenderTarget->Height; ++y)
        {

            /*gotoxy(1, 1 + i);
            fwrite(currentRenderTarget->DATA + i * currentRenderTarget->Width, 1, currentRenderTarget->Width, stdout);*/
        }
    }
    //fwrite(currentRenderTarget->DATA, 1, currentRenderTarget->Width * currentRenderTarget->Height, stdout);
    //fflush(stdout);
}

