// ~~~ OpenGL & Win32 ~~~
// Tutorial No.24 - ODE Physics
// Alan Baylis 2006

#include <windows.h>
#include <ode/ode.h>    // ode library header
#include "shared.h"
#include "general.h"
#include "vector.h"
#include "vertex.h"
#include "quat.h"
#include "matrix.h"
#include "texture.h"
#include "locmath.h"
#include "polygon.h"
#include "object.h"
#include "camera.h"
#include "light.h"
#include "collision.h"
#include "winfuncs.h"
#include "timer.h"      // replaced the old GetTickCount timer
#include "ode.h"        // ode related functions
#include "console.h"
#include "resource.h"

// ODE Physics
MATRIX GeomMatrix;          // transformation matrix for geom rendering
dWorldID World;             // the ode simulation world 
dSpaceID Space;             // collision space
MyObject Object;            // one geom object
dJointGroupID contactgroup; // contact group for collisions

// Windows
static HGLRC hRC;
static HDC hDC;
HWND hWnd;
RECT screen;
PAINTSTRUCT ps;

// Math
float pi = 3.141592;
float radian = pi / 180;
float epsilon = 0.05;       // minimum collision distance

// Input
bool key[256];
bool released_key[256];

// Cameras
int currentCamera = 0;
int numCameras = 4;
float step = 5.0;

// Timing
Timer* MyTimer = new Timer;
double multiplier;

// Lights
int GL_LIGHT[8] = {GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7};    // Represent GL lights in a conveniant array
int currentLight = 0;
int numLights = 0;

// Cube polygons
POLYGON * polygon = new POLYGON[12];

// Texture
TEXTURE * texture = new TEXTURE[1];

// Camera
CAMERA * camera = new CAMERA[numCameras + 1];

// Lighting
LIGHT * light = new LIGHT[numLights + 1];
float lightColor[3] = {1.0, 1.0, 1.0};

// Dialogs
int DialogInUse = 0;

// Console
ConsoleWindow * Console = new ConsoleWindow;

void InitGL(int Width, int Height)
{
    InitODE();             // Initalize the ODE simulation
    SetGLProperties();
    SetGLMaterial();
    SetGLView(Width, Height);
    SetGLLighting(light);
    SetGLWorld(polygon);
    SetGLTexture(texture);
    SetGLCamera(camera);    
}

void ReSizeGLScene(int Width, int Height)
{
    SetGLView(Width, Height);
}

void DrawGLScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera[currentCamera].Update();
    // Collision detection goes here
    camera[currentCamera].Apply();

    int loop;
    for(loop = 0; loop <= numLights; loop++)
        light[loop].Apply();

    DrawGrid();

    SimLoop();   // Step the simulation last
}

LRESULT CALLBACK WndProc( HWND    hWnd,
                          UINT    message,
                          WPARAM    wParam,
                          LPARAM    lParam)
{
    GLuint    PixelFormat;
    static    PIXELFORMATDESCRIPTOR pfd=
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        16,
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        16,
        0,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };


    switch (message)
    {
        case WM_CREATE:
            hDC = GetDC(hWnd);

            PixelFormat = ChoosePixelFormat(hDC, &pfd);

            if (!PixelFormat)
            {
                MessageBox(NULL,"Can't find a suitable PixelFormat.","Error",MB_OK|MB_ICONERROR);
                PostQuitMessage(0);
                break;
            }

            if(!SetPixelFormat(hDC,PixelFormat,&pfd))
            {
                MessageBox(NULL,"Can't set the PixelFormat.","Error",MB_OK|MB_ICONERROR);
                PostQuitMessage(0);
                break;
            }

            hRC = wglCreateContext(hDC);
            if(!hRC)
            {
                MessageBox(NULL,"Can't create a GL Rendering Context.","Error",MB_OK|MB_ICONERROR);
                PostQuitMessage(0);
                break;
            }

            if(!wglMakeCurrent(hDC, hRC))
            {
                MessageBox(NULL,"Can't activate the GL Rendering Context.","Error",MB_OK|MB_ICONERROR);
                PostQuitMessage(0);
                break;
            }

            InitGL(screen.right, screen.bottom);
            camera[currentCamera].Position.x = 0.0;
            camera[currentCamera].Position.y = 3.0;
            camera[currentCamera].Position.z = 5.0;
        break;

        case WM_SYSCOMMAND:
        {
            switch (wParam)
            {
                case SC_SCREENSAVE:
                case SC_MONITORPOWER:
                    return 0;
            }
            break;
        }

        case WM_DESTROY:
        case WM_CLOSE:
            delete[] texture;
            delete[] polygon;
            delete[] camera;
            delete[] light;
            ChangeDisplaySettings(NULL, 0);

            wglMakeCurrent(hDC,NULL);
            wglDeleteContext(hRC);
            ReleaseDC(hWnd,hDC);

            PostQuitMessage(0);
        break;

        case WM_KEYDOWN:
            key[wParam] = TRUE;
        break;

        case WM_KEYUP:
            key[wParam] = FALSE;
        break;

        case WM_SIZE:
            SetCursorPos((int)(screen.right * 0.5), (int)(screen.bottom * 0.5));
            ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));
        break;

        case WM_MOUSEMOVE:
            camera[currentCamera].Delta_x = float(HIWORD(lParam) - screen.bottom * 0.5) * 10;
            camera[currentCamera].Delta_y = float(LOWORD(lParam) - screen.right * 0.5) * 10;
        break;

        case WM_LBUTTONDOWN:
            camera[currentCamera].Delta_z = -120.0;
        break;

        case WM_RBUTTONDOWN:
            camera[currentCamera].Delta_z = 120.0;
        break;

        case WM_LBUTTONUP:
            if (wParam != MK_RBUTTON)
                camera[currentCamera].Delta_z = 0.0;
        break;

        case WM_RBUTTONUP:
            if (wParam != MK_LBUTTON)
                camera[currentCamera].Delta_z = 0.0;
        break;

        default:
            return (DefWindowProc(hWnd, message, wParam, lParam));
    }

    return (0);
}

int WINAPI WinMain( HINSTANCE    hInstance,
                    HINSTANCE,
                    LPSTR,
                    int)
{
    MSG           msg;
    WNDCLASSEX    wc;

    GetWindowRect(GetDesktopWindow(), &screen);
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style            = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_SAVEBITS;
    wc.lpfnWndProc      = (WNDPROC) WndProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hInstance        = hInstance;
    wc.hIcon            = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYICON));
    wc.hIconSm          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYICON));
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = NULL;
    wc.lpszMenuName     = NULL;
    wc.lpszClassName    = "OpenGL WinClass";


    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL,"Failed To Register The Window Class.","Error",MB_OK|MB_ICONERROR);
        return FALSE;
    }

    hWnd = CreateWindowEx(
    WS_EX_LEFT,
    "OpenGL WinClass",
    "OpenGL & Win32 Tutorial No.24",
    WS_MAXIMIZE |
    WS_CLIPCHILDREN |
    WS_CLIPSIBLINGS |
    WS_POPUPWINDOW |
    WS_VISIBLE,
    0, 0,
    screen.right, screen.bottom,
    NULL,
    NULL,
    hInstance,
    NULL);

    if(!hWnd)
    {
        MessageBox(NULL,"Window Creation Error.","Error",MB_OK|MB_ICONERROR);
        return FALSE;
    }

    DEVMODE dmScreenSettings;

    memset(&dmScreenSettings, 0, sizeof(DEVMODE));
    dmScreenSettings.dmSize        = sizeof(DEVMODE);
    dmScreenSettings.dmPelsWidth    = screen.right;
    dmScreenSettings.dmPelsHeight    = screen.bottom;
    dmScreenSettings.dmFields    = DM_PELSWIDTH | DM_PELSHEIGHT;
    ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

    ShowWindow(hWnd, SW_HIDE);

    Console->Open();
    SetConsoleTitle("Basic ODE Example");
    cout << "Pressing the spacebar drops a new box\n\r" << endl;
    cout << "The mouse and arrow keys control the camera\n\r" << endl;
    system("PAUSE");
    Console->Close();
    
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
    SetFocus(hWnd);
    SetCursorPos((int)(screen.right * 0.5), (int)(screen.bottom * 0.5));
    ShowCursor(0);
    
    while (1)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
        {
            if (GetMessage(&msg, NULL, 0, 0))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else
            {
                return TRUE;
            }
        }

        SetCursorPos((int)(screen.right * 0.5), (int)(screen.bottom * 0.5));
        DrawGLScene();
        glFlush();
        SwapBuffers(hDC);

        MyTimer->Refresh();
        multiplier = MyTimer->frametime;
        camera[currentCamera].Multiplier = multiplier;
        light[currentLight].Multiplier = multiplier;

        // Reset the geom Object    
        if (key[VK_SPACE] && released_key[VK_SPACE] == 0)
        {
            dMatrix3 R;
            dBodySetPosition(Object.Body, 0, 10, -5);
            VECTOR tempVect(0.0, 0.0, 0.0);
            dBodySetLinearVel(Object.Body, tempVect.x, tempVect.y, tempVect.z);
            dRFromAxisAndAngle(R, dRandReal() * 2.0 - 1.0, dRandReal() * 2.0 - 1.0, dRandReal() * 2.0 - 1.0, dRandReal() * 10.0 - 5.0);
            dBodySetRotation(Object.Body, R);

            released_key[VK_SPACE] = 1;
        }

        if (!key[VK_SPACE])
            released_key[VK_SPACE] = 0;

        if (key[49])
        {
            step = 1.0;
        }

        if (key[50])
        {
            step = 2.0;
        }

        if (key[51])
        {
            step = 3.0;
        }

        if (key[52])
        {
            step = 4.0;
        }

        if (key[53])
        {
            step = 5.0;
        }

        if (key[54])
        {
            step = 6.0;
        }

        if (key[55])
        {
            step = 7.0;
        }

        if (key[56])
        {
            step = 8.0;
        }

        if (key[57])
        {
            step = 9.0;
        }

        if (key[VK_RIGHT])
        {
            camera[currentCamera].Movement_x += step;
        }

        if (key[VK_LEFT])
        {
            camera[currentCamera].Movement_x -= step;
        }

        if (key[VK_DOWN])
        {
            camera[currentCamera].Movement_z += step;
        }

        if (key[VK_UP])
        {
            camera[currentCamera].Movement_z -= step;
        }

        if (key[VK_PRIOR])
        {
            camera[currentCamera].Movement_y += step;
        }

        if (key[VK_NEXT])
        {
            camera[currentCamera].Movement_y -= step;
        }

        if (key[VK_ESCAPE])
            SendMessage(hWnd,WM_CLOSE,0,0);
    }
}



                                      


