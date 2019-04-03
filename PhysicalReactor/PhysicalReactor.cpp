// PhysicalReactor.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "PhysicalReactor.h"
#include "World/GameWorld.h"
#include "MISC/Timer.h"
#include<vld.h>

#define MAX_LOADSTRING 100
using namespace PRE;

GameWorld* gw;
Timer *gametimer;
bool GamePause = false;
bool WindowsMin = false;
bool WindowsMax = false;
bool Resizing = false;

HINSTANCE hInst;                               
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];            


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

    // TODO: 在此处放置代码。
	
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PHYSICALREACTOR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

  
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PHYSICALREACTOR));

	MSG msg = {0};
	gametimer->Reset();

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			if (!GamePause)
			{
				gametimer->Tick();
				gw->Update(gametimer->GetDeltaTime());
				gw->Render();
			}
		}
	}

    return (int) msg.wParam;
}




ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PHYSICALREACTOR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PHYSICALREACTOR);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      0, 0, 1920, 1080, nullptr, nullptr, hInstance, nullptr);
   if (!hWnd)
   {
      return FALSE;
   }


   gw = new GameWorld(hWnd);
   gametimer = new Timer;
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
			{
				delete gw;
				delete gametimer;
				DestroyWindow(hWnd);
			}
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			GamePause = true;
			gametimer->Stop();
		}
		else
		{
			GamePause = false;
			gametimer->Start();
		}
		break;
	case WM_SIZE:
	    {
		    if (Renderer::GetDevice() != nullptr)
		    {
				int width = LOWORD(lParam);
				int height = LOWORD(lParam);

				if (wParam==SIZE_MINIMIZED)
				{
					GamePause = true;
					WindowsMin = true;
					WindowsMax = false;
				}
				else if (wParam == SIZE_MAXIMIZED)
				{
					GamePause = false;
					WindowsMin = false;
					WindowsMax = true;
					gw->ReSize(width, height);
				}
				else if (wParam == SIZE_RESTORED)
				{
					if (WindowsMin)
					{
						GamePause = false;
						WindowsMin = false;
						gw->ReSize(width, height);
					}
					else if (WindowsMax)
					{
						GamePause = false;
						WindowsMax = false;
						gw->ReSize(width, height);
					}
				}
				else
				{
					gw->ReSize(width, height);
				}
				
		    }
	    } 
		break;
	case WM_ENTERSIZEMOVE:
	     {
		    GamePause = true;
			Resizing = true;
			gametimer->Stop();
	     }
		 break;
	case WM_EXITSIZEMOVE:
	    {
		   int width = LOWORD(lParam);
		   int height = LOWORD(lParam);
		    GamePause = false;
			Resizing = false;
			gametimer->Start();
			gw->ReSize(width, height);
	    }
	case WM_KEYDOWN:
	    {
		   switch (wParam)
		   {
		   case 87:
			gw->MoveForward(1.0f);
			break;
		   case 83:
			gw->MoveForward(-1.0f);
			break;
		   case 65:
			gw->MoveRight(-1.0f);
			break;
		   case 68:
			gw->MoveRight(1.0f);
			break;
		   default:
			 break;
		   }
	    }
		break;
	case WM_RBUTTONDOWN:
	    {
		   gw->MouseButtonDown(hWnd,LOWORD(lParam), HIWORD(lParam));
	    }
		break;
	case WM_RBUTTONUP:
	   {
		   gw->MouseButtonUp();
	   }
	case WM_MOUSEMOVE:
	    {
		  int x = LOWORD(lParam);
		  int y = HIWORD(lParam);
		  gw->CameraRotation(wParam,x, y);
	    }
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
           
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
