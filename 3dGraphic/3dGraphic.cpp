﻿// 3dGraphic.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "3dGraphic.h"
#include "Math/mymath.h"
#include "MyDirectX/Device.h"

#include "TestVS.h"

#define MAX_LOADSTRING 100
#define VIDEOCASHMEMORYSIZE 100000000

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
Device* device;

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
    device = new Device(VIDEOCASHMEMORYSIZE);
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

void func()
{
    //SetPixelShader();
    //SetVertexShader();

    //SetVertexBuffer(v1);
    //SetIndexBuffer(i1);
    //Draw();

    //SetVertexBuffer(v2);
    //SetIndexBuffer(i2);
    //Draw();


}


TestVS testVS;
VertexBuffer vertexBuffer;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
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

            float3 verticesPos[] =
            {
                -1, -1, 1,
                0, 1, 1,
                1, -1, 1
            };

            vertexBuffer.buffer = verticesPos;
            vertexBuffer.vertexSize = sizeof(float3);
            vertexBuffer.verticesCount = sizeof(verticesPos) / sizeof(float3);

            device->SetViewPort(viewPort);
            device->SetVertexBuffer(&vertexBuffer);
            device->SetVertexShader(&testVS);
            
            device->Draw(3);


            //for(float x = 0; x < width; x++)
            //    for (float y = 0; y < height; y++)
            //    {
            //        float3 color = shader(float2{x, height - y}, width, height) * 255;

            //        if (color.x <= 0) color.x = 0;
            //        if (color.y <= 0) color.y = 0;
            //        if (color.z <= 0) color.z = 0;

            //        SetPixel(hdc, x, y, RGB((int)color.x, (int)color.y, (int)color.z));
            //    }

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