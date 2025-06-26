#include "framework.h"
#include "linia1.h"
#include <objidl.h>
#include <gdiplus.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <commctrl.h>
#include <cstring>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
#pragma comment(lib, "Comctl32.lib")

#define MAX_LOADSTRING 100
#define ID_SLIDER_SPEED 2001
#define ID_SLIDER_MOVE 2002
#define ID_BUTTON_MODE0 3001
#define ID_BUTTON_MODE1 3002
#define ID_BUTTON_MODE2 3003
#define ID_BUTTON_MODE3 3004
#define ID_BUTTON_MODE4 3005

class MovingShape {
public:
    float x, y;
    float dx, dy;
    float size;
    Color color;
    static float speedMultiplier;

    MovingShape(float startX, float startY, float dx, float dy, float size, Color color)
        : x(startX), y(startY), dx(dx), dy(dy), size(size), color(color) {
    }

    virtual void Move() {
        x += dx * speedMultiplier*0.25;
        y += dy * speedMultiplier*0.25;
    }
    virtual void Draw(Graphics& g) = 0;

    static void SetSpeedMultiplier(float multiplier) {
        speedMultiplier = multiplier;
    }

    virtual const char* GetType() = 0;

    bool IsInSieveArea() {

        return (x >= 650 && x <= 720 && y >= 290 && y <= 300);
    }

    virtual ~MovingShape() {}
};

float MovingShape::speedMultiplier = 1.0f;

class MovingSquare : public MovingShape {
public:
    using MovingShape::MovingShape;

    void Move() override {
        if (x <= 690 && y <= 250) { dx = -1.0f; dy = 2.0f; }
        if (x <= 635) { dx = -2.0f; dy = -0.75f; }
        if (x <= 340) { dx = -1.0f; dy = 2.0f; }
        if (y >= 350 && x <= 400) { dx = -2.0f; dy = 0; }
        MovingShape::Move();
    }

    void Draw(Graphics& g) override {
        SolidBrush brush(color);
        g.FillRectangle(&brush, x, y, size, size);
    }

    const char* GetType() override { return "square"; }
};

class MovingTriangle : public MovingShape {
public:
    using MovingShape::MovingShape;

    void Move() override {
        if (x <= 690 && y <= 250) { dx = -1.0f; dy = 2.0f; }
        if (x <= 635) { dx = -2.0f; dy = -0.75f; }
        if (x <= 340) { dx = -1.0f; dy = 2.0f; }
        if (y >= 350 && x<=400) { dx = -2.0f; dy = 0; }
        MovingShape::Move();
    }

    void Draw(Graphics& g) override {
        SolidBrush brush(color);
        Point points[3] = {
            Point((INT)x, (INT)(y + size)),
            Point((INT)(x + size / 2), (INT)y),
            Point((INT)(x + size), (INT)(y + size))
        };
        g.FillPolygon(&brush, points, 3);
    }

    const char* GetType() override { return "triangle"; }
};

class MovingCircle : public MovingShape {
public:
    using MovingShape::MovingShape;

    void Move() override {
        if (x <= 690 && y <= 250) { dx = -1.0f; dy = 2.0f; }
        if (x <= 635) { dx = -2.0f; dy = -0.75f; }
        if (x <= 340) { dx = -1.0f; dy = 2.0f; }
        if (y >= 350 && x <= 400) { dx = -2.0f; dy = 0; }
        MovingShape::Move();
    }

    void Draw(Graphics& g) override {
        SolidBrush brush(color);
        g.FillEllipse(&brush, x, y, size, size);
    }

    const char* GetType() override { return "circle"; }
};

std::vector<MovingShape*> shapes;
int shapeTimerCounter = 0;
int spawnSpeed = 10;
int mode = 0;

void CreateRandomShape() {
    int type = rand() % 3;
    float startX = 1100.0f, startY = 390.0f;
    float dx = -2.0f, dy = -0.75f;
    float size = 20.0f;
    Color color(255, rand() % 256, rand() % 256, rand() % 256);

    if (mode == 1) {
        type = (rand() % 2 == 0) ? 0 : 2;
    }
    if (mode == 2) {
        type = (rand() % 2 == 0) ? 0 : 2;
    }
    if (mode == 3) {
        type = (rand() % 2 == 0) ? 0 : 1;
    }
    if (mode == 4) {
        type = (rand() % 2 == 0) ? 0 : 1;
    }

    switch (type) {
    case 0: shapes.push_back(new MovingSquare(startX, startY, dx, dy, size, color)); break;
    case 1: shapes.push_back(new MovingTriangle(startX, startY, dx, dy, size, color)); break;
    case 2: shapes.push_back(new MovingCircle(startX, startY, dx, dy, size, color)); break;
    }
}

void DrawScene(Graphics& graphics) {
    Pen pen(Color(255, 0, 0, 0));
    SolidBrush brush1(Color(255, 50, 50, 50));
    SolidBrush brush2(Color(255, 255, 200, 200));
    FontFamily fontfamily(L"Arial");
    Font font(&fontfamily, 16, FontStyleRegular, UnitPixel);

    graphics.Clear(Color::White);

    Point w1[4] = { Point(1100,400), Point(1100,415), Point(700,265), Point(700,250) };
    graphics.FillPolygon(&brush1, w1, 4);
    Point w2[4] = { Point(750,400), Point(750,415), Point(350,265), Point(350,250) };
    graphics.FillPolygon(&brush1, w2, 4);

    graphics.FillRectangle(&brush1, 0, 370, 385, 15);
    graphics.DrawString(L"sitko", -1, &font, PointF(663, 280), &brush1);
    graphics.FillRectangle(&brush2, 650, 300, 70, 10);

    for (auto it = shapes.begin(); it != shapes.end();) {
        MovingShape* s = *it;

        if (mode == 1) {
            if (strcmp(s->GetType(), "triangle") == 0) {
                delete s;
                it = shapes.erase(it);
                continue;
            }
            if (strcmp(s->GetType(), "square") == 0 && s->IsInSieveArea()) {
                delete s;
                it = shapes.erase(it);
                continue;
            }
        }
        if (mode == 2) {
            if (strcmp(s->GetType(), "triangle") == 0) {
                delete s;
                it = shapes.erase(it);
                continue;
            }
            if (strcmp(s->GetType(), "circle") == 0 && s->IsInSieveArea()) {
                delete s;
                it = shapes.erase(it);
                continue;
            }
        }
        if (mode == 3) {

            if (strcmp(s->GetType(), "circle") == 0) {
                delete s;
                it = shapes.erase(it);
                continue;
            }
            if (strcmp(s->GetType(), "triangle") == 0 && s->IsInSieveArea()) {
                delete s;
                it = shapes.erase(it);
                continue;
            }
        }
        if (mode == 4) {
            if (strcmp(s->GetType(), "circle") == 0) {
                delete s;
                it = shapes.erase(it);
                continue;
            }
            if (strcmp(s->GetType(), "square") == 0 && s->IsInSieveArea()) {
                delete s;
                it = shapes.erase(it);
                continue;
            }
        }

        s->Draw(graphics);
        ++it;
    }
}

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    srand((unsigned int)time(NULL));
    InitCommonControls();

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LINIA1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    if (!InitInstance(hInstance, nCmdShow)) return FALSE;

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LINIA1));
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    for (auto shape : shapes) delete shape;
    GdiplusShutdown(gdiplusToken);
    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex = { sizeof(WNDCLASSEX) };
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = szWindowClass;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LINIA1));
    wcex.lpszMenuName = MAKEINTRESOURCE(IDC_LINIA1);
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
    hInst = hInstance;
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 1280, 720, nullptr, nullptr, hInstance, nullptr);
    if (!hWnd) return FALSE;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static Bitmap* buffer = nullptr;
    static Graphics* bufferGraphics = nullptr;
    static HWND hSliderSpeed, hSliderMove;
    static HWND hButtonMode0, hButtonMode1, hButtonMode2, hButtonMode3, hButtonMode4;

    switch (message) {
    case WM_CREATE:
        SetTimer(hWnd, 1, 10, NULL);

        hSliderSpeed = CreateWindowEx(0, TRACKBAR_CLASS, L"", WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_BOTH,
            10, 10, 200, 30, hWnd, (HMENU)ID_SLIDER_SPEED, hInst, NULL);
        SendMessage(hSliderSpeed, TBM_SETRANGE, TRUE, MAKELPARAM(1, 50));
        SendMessage(hSliderSpeed, TBM_SETPOS, TRUE, 51 - spawnSpeed);

        CreateWindowW(L"STATIC", L"MIN", WS_CHILD | WS_VISIBLE, 10, 40, 30, 20, hWnd, NULL, hInst, NULL);
        CreateWindowW(L"STATIC", L"MAX", WS_CHILD | WS_VISIBLE, 180, 40, 30, 20, hWnd, NULL, hInst, NULL);
        CreateWindowW(L"STATIC", L"Prędkość pojawiania się elementów", WS_CHILD | WS_VISIBLE,
            220, 10, 300, 20, hWnd, NULL, hInst, NULL);

        hSliderMove = CreateWindowEx(0, TRACKBAR_CLASS, L"", WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_BOTH,
            10, 60, 200, 30, hWnd, (HMENU)ID_SLIDER_MOVE, hInst, NULL);
        SendMessage(hSliderMove, TBM_SETRANGE, TRUE, MAKELPARAM(0.1, 10));
        SendMessage(hSliderMove, TBM_SETPOS, TRUE, 0.1);

        CreateWindowW(L"STATIC", L"MIN", WS_CHILD | WS_VISIBLE, 10, 90, 30, 20, hWnd, NULL, hInst, NULL);
        CreateWindowW(L"STATIC", L"MAX", WS_CHILD | WS_VISIBLE, 180, 90, 30, 20, hWnd, NULL, hInst, NULL);
        CreateWindowW(L"STATIC", L"Prędkość ruchu elementów", WS_CHILD | WS_VISIBLE,
            220, 60, 300, 20, hWnd, NULL, hInst, NULL);

        hButtonMode0 = CreateWindowW(L"BUTTON", L"Tryb 0", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            10, 120, 180, 25, hWnd, (HMENU)ID_BUTTON_MODE0, hInst, NULL);
        hButtonMode1 = CreateWindowW(L"BUTTON", L"Tryb 1", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            200, 120, 180, 25, hWnd, (HMENU)ID_BUTTON_MODE1, hInst, NULL);
        hButtonMode2 = CreateWindowW(L"BUTTON", L"Tryb 2", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            390, 120, 180, 25, hWnd, (HMENU)ID_BUTTON_MODE2, hInst, NULL);
        hButtonMode3 = CreateWindowW(L"BUTTON", L"Tryb 3", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            580, 120, 180, 25, hWnd, (HMENU)ID_BUTTON_MODE3, hInst, NULL);
        hButtonMode4 = CreateWindowW(L"BUTTON", L"Tryb 4", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            770, 120, 180, 25, hWnd, (HMENU)ID_BUTTON_MODE4, hInst, NULL);

        buffer = new Bitmap(1280, 720, PixelFormat32bppARGB);
        bufferGraphics = Graphics::FromImage(buffer);
        break;

    case WM_HSCROLL:
    {
        HWND hwndCtl = (HWND)lParam;
        int pos = 0;

        if (hwndCtl == hSliderSpeed) {
            pos = SendMessage(hSliderSpeed, TBM_GETPOS, 0, 0);
            spawnSpeed = 51 - pos;
        }
        else if (hwndCtl == hSliderMove) {
            pos = SendMessage(hSliderMove, TBM_GETPOS, 0, 0);
            MovingShape::SetSpeedMultiplier((float)pos);
        }
    }
    break;
    case WM_TIMER:
    {
        shapeTimerCounter++;
        if (shapeTimerCounter >= spawnSpeed*2) {
            CreateRandomShape();
            shapeTimerCounter = 0;
        }

        for (auto shape : shapes) {
            shape->Move();
        }

        for (auto it = shapes.begin(); it != shapes.end();) {
            if ((*it)->x + (*it)->size < 0) {
                delete* it;
                it = shapes.erase(it);
            }
            else {
                ++it;
            }
        }

        RECT drawingArea = { 0, 200, 1280, 720 };
        InvalidateRect(hWnd, &drawingArea, FALSE);
    }
    break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_SLIDER_SPEED:
            spawnSpeed = 51 - SendMessage(hSliderSpeed, TBM_GETPOS, 0, 0);
            break;
        case ID_SLIDER_MOVE:
            MovingShape::SetSpeedMultiplier((float)SendMessage(hSliderMove, TBM_GETPOS, 0, 0));
            break;
        case ID_BUTTON_MODE0:
            mode = 0;
            shapes.clear();
            break;
        case ID_BUTTON_MODE1:
            mode = 1;
            shapes.clear();
            break;
        case ID_BUTTON_MODE2:
            mode = 2;
            shapes.clear();
            break;

        case ID_BUTTON_MODE3:
            mode = 3;
            shapes.clear();
            break;

        case ID_BUTTON_MODE4:
            mode = 4;
            shapes.clear();
            break;
        }
        


        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        bufferGraphics->Clear(Color::White);
        DrawScene(*bufferGraphics);

        Graphics graphics(hdc);
        graphics.DrawImage(buffer, 0, 0);

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        for (auto shape : shapes) {
            delete shape;
        }
        shapes.clear();

        delete bufferGraphics;
        delete buffer;

        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
