// 3dGraphic.cpp : Определяет точку входа для приложения.
//
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "framework.h"
#include "3dGraphic.h"
#include "Math/mymath.h"
#include "MyDirectX/Device.h"



#include "TestVS.h"
#include "TestPS.h"
#include "MyDirectX/Input_Layout.h"

#include "MyDirectX/Model.h"
//#include "../Include/assimp/Importer.hpp"
//#include "../Include/assimp/scene.h"
//#include "../Include/assimp/postprocess.h"

//#pragma comment (lib, "assimp-vc143-mtd.lib")




#define MAX_LOADSTRING 100
#define VIDEOCASHMEMORYSIZE 100000000

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
Device* device;
SwapChain* swapChain;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY3DGRAPHIC, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY3DGRAPHIC));
    


    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY3DGRAPHIC));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY3DGRAPHIC);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{


   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);





   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//





float3 DrawTriangle(float2 pixelPos, float3 A, float3 B, float3 C)
{
    float3 a = B - A;
    float3 b = C - A;
    float x = pixelPos.x;
    float y = pixelPos.y;

    float delta = -(a.x * b.y) - (a.z * b.x * y) - (a.y * b.z * x) + (x * b.y * a.z) + (b.z * y * a.x) + (a.y * b.x);

    if (delta == 0)
        return float3{ 0, 0, 0 };

    float deltaU = (A.x * b.y) + (A.z * b.x * y) + (A.y * b.z * x) - (A.z * b.y * x) - (A.y * b.x) - (A.x * b.z * y);
    float deltaV = (a.x * A.y) + (A.x * y * a.z) + (a.y * A.z * x) - (a.z * A.y * x) - (a.y * A.x) - (A.z * y * a.x);
    float deltaZ = -(A.z * b.y * a.x) - (a.z * b.x * A.y) - (a.y * b.z * A.x) + (A.x * b.y * a.z) + (A.y * b.z * a.x) + (A.z * a.y * b.x);

    float u = deltaU / delta;
    float v = deltaV / delta;
    float z = deltaZ / delta;

    if ((u + v <= 1) && (u >= 0) && (u <= 1) && (v >= 0) && (v <= 1) && (1 - u - v >= 0) && (1 - u - v <= 1))
    {
        //return float3{ u / z, v / z, (1.0f - u - v) / z };
        return float3{ 1.0f / z, 1.0f / z, 1.0f / z };
    }
    return float3{0, 0, 0};
}

float3 shader(float2 inputPos, float width, float height)
{
    float2 pixelPos = float2{ (inputPos.x / width) * 2 - 1, (inputPos.y / height) * 2 - 1 };


    float3 color = DrawTriangle(pixelPos, float3{ -1, -1, 5 }, float3{ 0, 1, 1 }, float3{1, -1, 2});

    //float3 color = DrawTriangle(pixelPos, float3{ -1, -1, 1 }, float3{ -1, 1, 1 }, float3{ 1, -1, 1 });
    return float3{ color.x, color.y, color.z};
}


TestVS testVS;
VertexBuffer vertexBuffer;
TestPS testPS;
Input_Layout inputLayout;
Texture2D depthStencil;
IndexBuffer indexBuffer;

struct VERTEX
{
    float3 pos;
    float4 color;
    float3 normal;
};


struct StructuredBuffer
{
    Matrix3x3 rotationMatrix;
    float3 objPos;
};

class Cube
{
public:
    float3 pos{};
    Matrix3x3 rotationMatrix{};
    Cube()
    {
        VERTEX vertices[] =
        {
            //bottom
            -1, -1, -1,  1, 1, 1, 1,    0, -1, 0,
            -1, -1, 1,  1, 1, 1, 1,     0, -1, 0,
            1 , -1, -1,  1, 1, 1, 1,     0, -1, 0,


            1, -1, -1,  1, 1, 1, 1,     0, -1, 0,
            1, -1, 1,  1, 1, 1, 1,      0, -1, 0,
            -1, -1, 1,  1, 1, 1, 1,     0, -1, 0,

            //left
             -1, -1, 1,  1, 1, 1, 1,     -1, 0, 0,
             -1, 1, 1,  1, 1, 1, 1,      -1, 0, 0,
             -1, 1, -1,  1, 1, 1, 1,     -1, 0, 0,

             -1, 1, -1,  1, 1, 1, 1,     -1, 0, 0,
             -1, -1, -1,  1, 1, 1, 1,    -1, 0, 0,
             -1, -1, 1,  1, 1, 1, 1,     -1, 0, 0,

             //back
              -1, -1, 1,  1, 1, 1, 1,      0, 0, 1,
              -1, 1, 1,  1, 1, 1, 1,       0, 0, 1,
              1, 1, 1,  1, 1, 1, 1,        0, 0, 1,

              1, 1, 1,  1, 1, 1, 1,        0, 0, 1,
              -1, -1, 1,  1, 1, 1, 1,      0, 0, 1,
              1, -1, 1,  1, 1, 1, 1,       0, 0, 1,

              //right
               1, -1, 1,  1, 1, 1, 1,      1, 0, 0,
               1, -1, -1,  1, 1, 1, 1,     1, 0, 0,
               1, 1, -1,  1, 1, 1, 1,      1, 0, 0,

               1, 1, -1,  1, 1, 1, 1,      0, 0, 0,
               1, -1, 1,  1, 1, 1, 1,      1, 0, 0,
               1, 1, 1,  1, 1, 1, 1,       1, 0, 0,

               //top
                1, 1, 1,  1, 1, 1, 1, 0, 1, 0,
                -1, 1, 1,  1, 1, 1, 1, 0, 1, 0,
                -1, 1, -1,  1, 1, 1, 1, 0, 1, 0,

               -1, 1, -1,  1, 1, 1, 1, 0, 1, 0,
               1, 1, 1,  1, 1, 1, 1, 0, 1, 0,
               1, 1, -1,  1, 1, 1, 1, 0, 1, 0,

               //front
                  1, 1, -1,  1, 1, 1, 1, 0, 0, -1,
                  -1, 1, -1,  1, 1, 1, 1,  0, 0, -1,
                  -1, -1, -1,  1, 1, 1, 1,  0, 0, -1,

                  -1, -1, -1,  1, 1, 1, 1,  0, 0, -1,
                  1, -1, -1,  1, 1, 1, 1,  0, 0, -1,
                  1, 1, -1,  1, 1, 1, 1,  0, 0, -1,
        };

        cubeVertexBuffer.vertexSize = sizeof(VERTEX);
        cubeVertexBuffer.verticesCount = sizeof(vertices) / sizeof(VERTEX);

        this->vertices = new VERTEX[cubeVertexBuffer.verticesCount];
        for (int i = 0; i < cubeVertexBuffer.verticesCount; i++)
            this->vertices[i] = vertices[i];

        cubeVertexBuffer.buffer = this->vertices;

    }
    void Draw(Device** device)
    {
        structuredBuffer.objPos = pos;
        structuredBuffer.rotationMatrix = rotationMatrix;
        (*device)->SetStructuredBuffer(&structuredBuffer);
        (*device)->SetVertexBuffer(&cubeVertexBuffer);
        (*device)->SetVertexShader(&testVS);
        (*device)->SetPixelShader(&testPS);
        (*device)->Draw(cubeVertexBuffer.verticesCount);
    }
private:
    VertexBuffer cubeVertexBuffer;
    VERTEX* vertices;
    StructuredBuffer structuredBuffer;

};

VERTEX* DrawCube(float3 pos, ViewPort viewPort)
{
    //Assimp::Importer importer;
    //float3 pos = { 3, -4, 8 };
    VERTEX vertices[] =
    {
        //bottom
        -1 + pos.x, -1 + pos.y, -1 + pos.z,  1, 1, 1, 1,    0, -1, 0,
        -1 + pos.x, -1 + pos.y, 1 + pos.z,  1, 1, 1, 1,     0, -1, 0,
        1 + pos.x, -1 + pos.y, -1 + pos.z,  1, 1, 1, 1,     0, -1, 0,


        1 + pos.x, -1 + pos.y, -1 + pos.z,  1, 1, 1, 1,     0, -1, 0,
        1 + pos.x, -1 + pos.y, 1 + pos.z,  1, 1, 1, 1,      0, -1, 0,
        -1 + pos.x, -1 + pos.y, 1 + pos.z,  1, 1, 1, 1,     0, -1, 0,

        // //left
         -1 + pos.x, -1 + pos.y, 1 + pos.z,  1, 1, 1, 1,     -1, 0, 0,
         -1 + pos.x, 1 + pos.y, 1 + pos.z,  1, 1, 1, 1,      -1, 0, 0,
         -1 + pos.x, 1 + pos.y, -1 + pos.z,  1, 1, 1, 1,     -1, 0, 0,

         -1 + pos.x, 1 + pos.y, -1 + pos.z,  1, 1, 1, 1,     -1, 0, 0,
         -1 + pos.x, -1 + pos.y, -1 + pos.z,  1, 1, 1, 1,    -1, 0, 0,
         -1 + pos.x, -1 + pos.y, 1 + pos.z,  1, 1, 1, 1,     -1, 0, 0,

         // //back
          -1 + pos.x, -1 + pos.y, 1 + pos.z,  1, 1, 1, 1,      0, 0, 1,
          -1 + pos.x, 1 + pos.y, 1 + pos.z,  1, 1, 1, 1,       0, 0, 1,
          1 + pos.x, 1 + pos.y, 1 + pos.z,  1, 1, 1, 1,        0, 0, 1,

          1 + pos.x, 1 + pos.y, 1 + pos.z,  1, 1, 1, 1,        0, 0, 1,
          -1 + pos.x, -1 + pos.y, 1 + pos.z,  1, 1, 1, 1,      0, 0, 1,
          1 + pos.x, -1 + pos.y, 1 + pos.z,  1, 1, 1, 1,       0, 0, 1,

          // //right
           1 + pos.x, -1 + pos.y, 1 + pos.z,  1, 1, 1, 1,      1, 0, 0,
           1 + pos.x, -1 + pos.y, -1 + pos.z,  1, 1, 1, 1,     1, 0, 0,
           1 + pos.x, 1 + pos.y, -1 + pos.z,  1, 1, 1, 1,      1, 0, 0,

           1 + pos.x, 1 + pos.y, -1 + pos.z,  1, 1, 1, 1,      1, 0, 0,
           1 + pos.x, -1 + pos.y, 1 + pos.z,  1, 1, 1, 1,      1, 0, 0,
           1 + pos.x, 1 + pos.y, 1 + pos.z,  1, 1, 1, 1,       1, 0, 0,

           // //top
            1 + pos.x, 1 + pos.y, 1 + pos.z,  1, 1, 1, 1, 0, 1, 0,
            -1 + pos.x, 1 + pos.y, 1 + pos.z,  1, 1, 1, 1, 0, 1, 0,
            -1 + pos.x, 1 + pos.y, -1 + pos.z,  1, 1, 1, 1, 0, 1, 0,

           -1 + pos.x, 1 + pos.y, -1 + pos.z,  1, 1, 1, 1, 0, 1, 0,
           1 + pos.x, 1 + pos.y, 1 + pos.z,  1, 1, 1, 1, 0, 1, 0,
           1 + pos.x, 1 + pos.y, -1 + pos.z,  1, 1, 1, 1, 0, 1, 0,

           //   ////front
              1 + pos.x, 1 + pos.y, -1 + pos.z,  1, 1, 1, 1, 0, 0, -1,
              -1 + pos.x, 1 + pos.y, -1 + pos.z,  1, 1, 1, 1,  0, 0, -1,
              -1 + pos.x, -1 + pos.y, -1 + pos.z,  1, 1, 1, 1,  0, 0, -1,

              -1 + pos.x, -1 + pos.y, -1 + pos.z,  1, 1, 1, 1,  0, 0, -1,
              1 + pos.x, -1 + pos.y, -1 + pos.z,  1, 1, 1, 1,  0, 0, -1,
              1 + pos.x, 1 + pos.y, -1 + pos.z,  1, 1, 1, 1,  0, 0, -1,
    };

    vertexBuffer.buffer = vertices;
    vertexBuffer.vertexSize = sizeof(VERTEX);
    vertexBuffer.verticesCount = sizeof(vertices) / sizeof(VERTEX);

    Matrix3x3 rotationMatrix;
    rotationMatrix.MakeUnit();

    rotationMatrix.RotateX(&rotationMatrix, 3.14f / 3);
    rotationMatrix.RotateY(&rotationMatrix, 3.14f / 4 - 0.3f);
    //rotationMatrix.RotateZ(&rotationMatrix, 3.14f / 4 - 0.3f);

    StructuredBuffer structuredBuffer;
    structuredBuffer.objPos = { 0, 0, 6 };
    structuredBuffer.rotationMatrix = rotationMatrix;

    device->SetStructuredBuffer(&structuredBuffer);
    device->SetVertexBuffer(&vertexBuffer);
    device->SetVertexShader(&testVS);
    device->SetPixelShader(&testPS);

    device->Draw(vertexBuffer.verticesCount);

    return vertices;
}

void DrawTriangle(ViewPort viewPort)
{
    VERTEX triangle[] =
    {
        -1, -1, 3,  1, 1, 1, 1,    0, 0, -1,
        0, 1, 3,  1, 1, 1, 1,    0, 0, -1,
        1, -1, 3,  1, 1, 1, 1,    0, 0, -1
    };
    VERTEX quard[]
    {
       -1, -1, 3,  1, 1, 1, 1,    0, 0, -1,
        -1, 1, 3,  1, 1, 1, 1,    0, 0, -1,
        1, 1, 3,  1, 1, 1, 1,    0, 0, -1,
        1, -1, 3,  1, 1, 1, 1,    0, 0, -1
    };
    vertexBuffer.buffer = quard;
    vertexBuffer.vertexSize = sizeof(VERTEX);
    vertexBuffer.verticesCount = sizeof(quard) / sizeof(VERTEX);

    //unsigned int indexes[] =
    //{
    //    0, 1, 2
    //};

    unsigned int indexes[] =
    {
        0, 1, 3,
        3, 2, 1
    };

    indexBuffer.Init(indexes, sizeof(indexes) / sizeof(unsigned int));

    Matrix3x3 rotationMatrix;




    device->SetViewPort(viewPort);
    device->SetVertexBuffer(&vertexBuffer);
    device->SetIndexBuffer(&indexBuffer);
    device->SetVertexShader(&testVS);
    device->SetPixelShader(&testPS);

    device->Draw(vertexBuffer.verticesCount);
}

Cube cube;
Model* monkeyModel;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        RECT clientRect{};
        GetClientRect(hWnd, &clientRect);

        float width = clientRect.right - clientRect.left;
        float height = clientRect.bottom - clientRect.top;

        device = new Device(VIDEOCASHMEMORYSIZE);
        device->CreateSwapChain(&swapChain, 2, float2{ width, height }, DXGI_FORMAT_R32G32B32A32_FLOAT);

        Attribute* attribute = new Attribute[2];

        attribute[0] = { (char*)"POSITION", DXGI_FORMAT_R32G32B32_FLOAT, 0 };
        //attribute[1] = { (char*)"COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT, 12 };
        attribute[1] = { (char*)"NORMAL", DXGI_FORMAT_R32G32B32_FLOAT, 12 };
        

        inputLayout.attributeBuffer = attribute;
        inputLayout.elementsCount = 2;

        device->SetInputLayout(&inputLayout);

        depthStencil.InitTexture2D(DXGI_FORMAT_R32_FLOAT, float2{ width, height });

        Matrix3x3 rotationMatrix;
        rotationMatrix.MakeUnit();

        rotationMatrix.RotateY(&rotationMatrix, 3.14f);
        rotationMatrix.RotateX(&rotationMatrix, 3.14f / 2);
        //rotationMatrix.RotateZ(&rotationMatrix, 3.14f / 4 - 0.3f);

        cube.pos = {0, 0, 6};
        cube.rotationMatrix = rotationMatrix;

        monkeyModel = new Model("../models/Monkey.obj");

        monkeyModel->rotationMatrix = rotationMatrix;
        monkeyModel->position = {0, 0, 2};


        //std::vector<int> test;

        //test.push_back(1);
        //test.push_back(2);

        //int* ptest = new int[2];

        //ptest = (int*)&test[0];
        //int point = 0;
        //for (int i = 0; i < 2; i++)
        //{
        //    point = ptest[i];
        //    int k = 0;
        //}


    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {

        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        RECT clientRect{};
        GetClientRect(hWnd, &clientRect);

        int width = clientRect.right - clientRect.left;
        int height = clientRect.bottom - clientRect.top;

        ViewPort viewPort;
        viewPort.left = clientRect.left;
        viewPort.top = clientRect.top;
        viewPort.right = clientRect.right;
        viewPort.bottom = clientRect.bottom;

        device->SetViewPort(viewPort);



        device->ClearBuffer(float4{ 117.0f / 255, 165.0f / 255, 231.0f / 255, 1 }, &swapChain->backBuffers[swapChain->currentBackBufferId]);
        device->SetDepthStencil(&depthStencil);

        //cube.Draw(&device);
        //cube.pos = { 1, 0, 6 };

        //cube.Draw(&device);

        monkeyModel->Draw(device, &testVS, &testPS );

        swapChain->Prevent();

        for (int x = viewPort.left; x < viewPort.right; x++)
            for (int y = viewPort.top; y < viewPort.bottom; y++)
            {
                float4 color = *(float4*)((char*)(swapChain)->backBuffers[(swapChain)->frontBufferId].texture2D[x] + (y * (swapChain)->backBuffers->format));

                if (color.x <= 0) color.x = 0;
                if (color.y <= 0) color.y = 0;
                if (color.z <= 0) color.z = 0;

                if (color.x > 1) color.x = 1;
                if (color.y > 1) color.y = 1;
                if (color.z > 1) color.z = 1;

                color.x *= 255.0f;
                color.y *= 255.0f;
                color.z *= 255.0f;
                SetPixel(hdc, x, y, RGB((int)color.x, (int)color.y, (int)color.z));
            }
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
