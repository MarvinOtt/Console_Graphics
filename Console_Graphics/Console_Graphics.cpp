#pragma once
#define OLC_PGE_APPLICATION
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <time.h>
#include "bse_console.h"
#include "olcPixelGameEngine.h"
#include "GraphicsDevice.h"

#define M_PI 3.14159265358979323846

void InizialiseByteArray(unsigned char* array, int count, unsigned char value)
{
    int i;
    for (i = 0; i < count; i++)
    {
        array[i] = value;
    }
}
void InizialiseFloatArray(float* array, int count, float value)
{
    int i;
    for (i = 0; i < count; i++)
    {
        array[i] = value;
    }
}

const int Screenwidth = 1280 / 1, Screenheight = 720 / 1;

class Example : public olc::PixelGameEngine
{
public:
    Vector3 camerapos = Vector3(0.5f, 0.5f, -1.0f);
    Vector3 cameradirection = Vector3(0, 0, 1);
    Vector2 newmousepos, oldmousepos, clickedmousepos;
    int newmouse_RBUTTON_clicked, oldmouse_RBUTTON_clicked;
    float camerarotationX = 0, camerarotationY = 0;
    float cameraspeed = 0.05f;
    int i, x, y, z, timer = 0;
    double lasttime = 0;

    GraphicsDevice graphicsdevice;
    RenderTarget maintarget;
    Matrix view, projection, VP;

    VertexBuffer triangle;
    VertexBuffer circle;

    Example()
    {
        sAppName = "Example";
        unsigned char ASCIIbrighness[1000];
        FILE * file;
        fopen_s(&file, "ASCII_brightness", "r");
        if (file != NULL)
        {
            fread(ASCIIbrighness, 1000, 1, file);
            fclose(file);
        }
        graphicsdevice = GraphicsDevice(ASCIIbrighness);
    }

public:

    bool OnUserCreate() override
    {
        
        

        maintarget = RenderTarget(Screenwidth, Screenheight);
        Vertex vertexes[24];
        vertexes[0].pos = Vector4(0, 0, 0, 1);
        vertexes[1].pos = Vector4(1, 0, 0, 1);
        vertexes[2].pos = Vector4(1, 0, 0, 1);
        vertexes[3].pos = Vector4(1, 1, 0, 1);
        vertexes[4].pos = Vector4(1, 1, 0, 1);
        vertexes[5].pos = Vector4(0, 1, 0, 1);
        vertexes[6].pos = Vector4(0, 1, 0, 1);
        vertexes[7].pos = Vector4(0, 0, 0, 1);

        vertexes[8].pos = Vector4(0, 0, 1, 1);
        vertexes[9].pos = Vector4(1, 0, 1, 1);
        vertexes[10].pos = Vector4(1, 0, 1, 1);
        vertexes[11].pos = Vector4(1, 1, 1, 1);
        vertexes[12].pos = Vector4(1, 1, 1, 1);
        vertexes[13].pos = Vector4(0, 1, 1, 1);
        vertexes[14].pos = Vector4(0, 1, 1, 1);
        vertexes[15].pos = Vector4(0, 0, 1, 1);

        vertexes[16].pos = Vector4(0, 0, 0, 1);
        vertexes[17].pos = Vector4(0, 0, 1, 1);
        vertexes[18].pos = Vector4(1, 0, 0, 1);
        vertexes[19].pos = Vector4(1, 0, 1, 1);
        vertexes[20].pos = Vector4(0, 1, 0, 1);
        vertexes[21].pos = Vector4(0, 1, 1, 1);
        vertexes[22].pos = Vector4(1, 1, 0, 1);
        vertexes[23].pos = Vector4(1, 1, 1, 1);

        vertexes[0].brighness = 1;
        vertexes[1].brighness = 0.1f;
        vertexes[2].brighness = 0.1f;
        vertexes[3].brighness = 1;
        vertexes[4].brighness = 1;
        vertexes[5].brighness = 0.1f;
        vertexes[6].brighness = 0.1f;
        vertexes[7].brighness = 1;

        vertexes[8].brighness = 0.1f;
        vertexes[9].brighness = 1;
        vertexes[10].brighness = 1;
        vertexes[11].brighness = 0.1f;
        vertexes[12].brighness = 0.1f;
        vertexes[13].brighness = 1;
        vertexes[14].brighness = 1;
        vertexes[15].brighness = 0.1f;

        vertexes[16].brighness = 1;
        vertexes[17].brighness = 0.1f;
        vertexes[18].brighness = 0.1f;
        vertexes[19].brighness = 1;
        vertexes[20].brighness = 1;
        vertexes[21].brighness = 0.1f;
        vertexes[22].brighness = 0.1f;
        vertexes[23].brighness = 1;

        /*vertexes[4].pos = Vector4_Init(0, 0, 1, 1);
        vertexes[5].pos = Vector4_Init(1, 0, 1, 1);
        vertexes[6].pos = Vector4_Init(0, 1, 1, 1);
        vertexes[7].pos = Vector4_Init(1, 1, 1, 1);*/

        /// Generating Vertex Buffer

        VertexBuffer object1(24);
        object1.SetData(vertexes);
        int countt = 0;
        Vertex *vertexes2 = new Vertex[22];
        for (i = 0; i < 11; ++i)
        {
            vertexes2[i * 2].pos = Vector4(sin((i / 11.0f) * M_PI * 2), 0, cos((i / 11.0f) * M_PI * 2), 1);
            vertexes2[i * 2].brighness = fabs(sin((i / 11.0f) * M_PI * 2));
            vertexes2[i * 2 + 1].pos = Vector4(sin(((i + 1) / 11.0f) * M_PI * 2), 0, cos(((i + 1) / 11.0f) * M_PI * 2), 1);
            vertexes2[i * 2 + 1].brighness = fabs(sin(((i + 1) / 11.0f) * M_PI * 2));
        }
        Vertex *vertexes3 = new Vertex[3];
        vertexes3[0].pos = Vector4(0, 0, 0, 1);
        vertexes3[1].pos = Vector4(2, 0, 0, 1);
        vertexes3[2].pos = Vector4(1, 1, 0, 1);
        vertexes3[0].brighness = 1.0f;
        vertexes3[1].brighness = 1.0f;
        vertexes3[2].brighness = 0.0f;

        triangle = VertexBuffer(3);
        triangle.SetData(vertexes3);

        circle = VertexBuffer(22);
        circle.SetData(vertexes2);

        /// Inizialising and Loading Main RenderTarget3D
        unsigned char *DATA = new unsigned char[maintarget.Width * maintarget.Height];
        float *DEPTH = new float[maintarget.Width * maintarget.Height];

        InizialiseByteArray(DATA, maintarget.Width * maintarget.Height, 0);
        InizialiseFloatArray(DEPTH, maintarget.Width * maintarget.Height, 999999.0f);
        maintarget.SetData(DATA, DEPTH);
        Vector4 pos = Vector4(1, 0, 1, 1);
        view = Matrix::CreateLookAt(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, -1, 0));
        ///470.0f / (190.0f * (5.0f / 4.0f))
        projection = Matrix::CreateFieldOfView(M_PI / 2.0f, (float)Screenwidth / ((float)Screenheight * (1.0f / 1.0f)), 0.1f, 100);
        VP = view * projection;
        pos = VP.TransposeVector4(pos);
        //pos = Matrix_multiply_Vector4(pos, projection);
        //Matrix_Print(projection);


        graphicsdevice.SetRenderTarget(&maintarget);
        graphicsdevice.SetVertexBuffer(&triangle);
        //#ifndef TESTING
        //    Vector4 finalpos = Vector4(-1, 0, 0.01f, 1);
        //    finalpos = Matrix::CreateTranslation(Vector3(0, 0, 0)).TransposeVector4(finalpos);
        //    finalpos = view.TransposeVector4(finalpos);
        //    finalpos = projectionTransposeVector4(finalpos, projection);
        //    float dot = Dot_Clipping(0, finalpos);
        //
        //    printf("\nPOS: (%.2f, %.2f, %.2f, %.2f)", finalpos.X, finalpos.Y, finalpos.Z, finalpos.W);
        //    printf("\nDOT: %.5f", dot);
        //
        //    finalpos = Vector4_Init(0, 0, 4, 1);
        //    finalpos = Matrix_TransposeVector4(finalpos, Matrix_CreateTranslation(Vector3_Init(0, 0, 0)));
        //    finalpos = Matrix_TransposeVector4(finalpos, view);
        //    finalpos = Matrix_TransposeVector4(finalpos, projection);
        //    dot = Dot_Clipping(0, finalpos);
        //    printf("\nDOT: %.5f", dot);
        //
        //    Vector3 postest = Vector3_Init(0, 10, 0);
        //    Matrix matrixtest = Matrix_CreateTranslation(Vector3_Init(1, 2, 3));
        //    postest = Matrix_TransposeVector3(postest, matrixtest);
        //
        //    //getch();
        //#endif

        float r = 0;
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {

        ///
        /// UPDATING CAMERA
        ///

        POINT curserpos;
        GetCursorPos(&curserpos);
        newmousepos.X = curserpos.x;
        newmousepos.Y = curserpos.y;
        newmouse_RBUTTON_clicked = (GetKeyState(VK_RBUTTON) & 0x8000);

        cameradirection.X = sin(camerarotationY) * cos(camerarotationX);
        cameradirection.Z = cos(camerarotationY) * cos(camerarotationX);
        cameradirection.Y = sin(camerarotationX);

        if (_kbhit() || true) /// Any Key was pressed
        {
            if (GetKeyState(VK_SHIFT) & 0x8000) /// Shift Key
                cameraspeed = 0.5f;
            else
                cameraspeed = 0.025f;
            if (GetKeyState(0x57) & 0x8000) /// W Key
            {
                camerapos = camerapos + cameradirection * cameraspeed;
            }
            if (GetKeyState(0x53) & 0x8000) /// S Key
            {
                camerapos = camerapos - cameradirection * cameraspeed;
            }
            if (GetKeyState(0x41) & 0x8000) /// A Key
            {
                camerapos.X -= cos(camerarotationY) * cameraspeed;
                camerapos.Z += sin(camerarotationY) * cameraspeed;
            }
            if (GetKeyState(0x44) & 0x8000) /// D Key
            {
                camerapos.X += cos(camerarotationY) * cameraspeed;
                camerapos.Z -= sin(camerarotationY) * cameraspeed;
            }
            if (GetKeyState(VK_SPACE) & 0x8000) /// Space Key
            {
                camerapos.Y += cameraspeed;
            }
            if (GetKeyState(VK_CONTROL) & 0x8000) /// Control Key
            {
                camerapos.Y -= cameraspeed;
            }
        }
        if (newmouse_RBUTTON_clicked)
        {
            if (!oldmouse_RBUTTON_clicked)
            {
                clickedmousepos = newmousepos;
            }
            camerarotationY -= (clickedmousepos.X - newmousepos.X) * 0.0035f;
            camerarotationX += (clickedmousepos.Y - newmousepos.Y) * 0.0035f;
            SetCursorPos(clickedmousepos.X, clickedmousepos.Y);
        }
        view = Matrix::CreateLookAt(camerapos, camerapos + cameradirection, Vector3(0, -1, 0));
        graphicsdevice.Clear(0);


        ///
        /// DRAWING OBJECTS
        ///
        for (z = 0; z < 3; z++)
        {
            for (x = 0; x < 20; x++)
            {
                for (y = 0; y < 20; y++)
                {
                    //Matrix_CreateTranslation(Vector3_Init(x / 0.1f, y / 0.1f, z / 0.1f)
                    //GraphicsDevice_DrawPrimitives(&graphicsdevice, 0, Matrix_CreateTranslation(Vector3_Init(x / 0.5f, y / 0.5f, z / 0.5f)) ,view, projection);
                    //graphicsdevice.DrawLine(&maintarget, 0, 0, 1, 1);
                    //GraphicsDevice_DrawPrimitives(&graphicsdevice, 0, Matrix_CreateTranslation(Vector3_Init(x / 0.5f + 100.0f, y / 0.5f, z / 0.5f)) ,view, projection);
                    graphicsdevice.DrawPrimitives(TRIANGLE_LIST, /*Matrix::CreateRotationX(lasttime * (y + x * 50) * 0.0000005f + lasttime * 0.0005f) * */Matrix::CreateTranslation(Vector3(x / 0.5f, y / 1.0f, z / 0.1f)), view, projection);
                    //GraphicsDevice_DrawPrimitives(&graphicsdevice, 3, Matrix_CreateTranslation(Vector3_Init(x / 1.0f, y / 1.0f + 0.5f, z / 1.0f + 0.01f)) ,view, projection);
                    //GraphicsDevice_DrawPrimitives(&graphicsdevice, 3, Matrix_multiply_Matrix(Matrix_CreateRotationX(r), Matrix_CreateTranslation(Vector3_Init(x, y, z - 0.5f))) ,view, projection);
                    //GraphicsDevice_DrawPrimitives(&graphicsdevice, 3, Matrix_multiply_Matrix(Matrix_CreateRotationX(-r*5), Matrix_CreateTranslation(Vector3_Init(x, y, z + 0.5f))) ,view, projection);
                    //GraphicsDevice_DrawPrimitives(&graphicsdevice, 0, Matrix_CreateRotationX((r) * M_PI*2),view, projection, timer);
                }
            }
        }
        Clear(olc::Pixel(0, 0, 0, 255));
        for (int x = 0; x < maintarget.Width; ++x)
        {
            for (int y = 0; y < maintarget.Height; ++y)
            {
                uint8_t val = maintarget.DATA[x + y * maintarget.Width];
                if(val != 0)
                    Draw(x, y, olc::Pixel(val, val, val, 255));
                
                //engine.Draw(x, y, olc::Pixel(255, 0, 255, 255));
                /*gotoxy(1, 1 + i);
                fwrite(currentRenderTarget->DATA + i * currentRenderTarget->Width, 1, currentRenderTarget->Width, stdout);*/
            }
        }

        
        //graphicsdevice.DrawOnScreen();
        double buf = lasttime;
        lasttime = clock();
        double lastms = (buf / (double)CLOCKS_PER_SEC);
        double newms = (lasttime / (double)CLOCKS_PER_SEC);
        double deltaT = newms - lastms;
        char str[20];
        sprintf_s(str, "%f", 1.0 / deltaT);
        wchar_t wtext[20];
        size_t outSize;
        mbstowcs_s(&outSize, wtext, strlen(str) + 1, str, strlen(str));//Plus null
        LPWSTR ptr = wtext;
        SetConsoleTitle(ptr);

        oldmousepos = newmousepos;
        oldmouse_RBUTTON_clicked = newmouse_RBUTTON_clicked;

        
        return true;
    }
};

int main()
{
    Example demo;
    if (demo.Construct(Screenwidth, Screenheight, 1, 1, false, false))
        demo.Start();

    return 0;
}
