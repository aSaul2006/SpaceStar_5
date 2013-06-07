#include <iostream>
using namespace std;

#include <windows.h>
#define VC_EXTRALEAN

#include "SpaceStar5.h"
#include <cstring>


///////////////////////////////////////////////////////////////////////////////
// Global Variables
///////////////////////////////////////////////////////////////////////////////

// screen resolution
const unsigned int SCREEN_HEIGHT = 600;
const unsigned int SCREEN_WIDTH = 800;

// window title
#define WINDOW_TITLE L"Space Star 5"

// windows global variables
HWND		g_hWnd;			// global handle to the window
HINSTANCE	g_hInst;		// global application instance
bool		g_bWindowed;	// windowed or fullscreen?

// Game Class
SpaceStar5		SS5Game;

// Entry point to the windows application
int WINAPI wWinMain(HINSTANCE hInstance,		// Handle to the app
					HINSTANCE hPrevInstance,	// Handle to previous app
					LPTSTR lpCmdLine,			// command line string
					int lpCmdShow);				// Show window value

// Windows Message Handler
LRESULT CALLBACK WndProc(HWND hWnd,		// Handle to the window
						UINT message,	// Incoming Message
						WPARAM wparam,	// Message info
						LPARAM lparam);	// Message info



// Initialize window creation
void InitWindow(void)
{
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));

	// set up the window class veriables
	wndClass.cbSize			= sizeof(wndClass);	// size of the window class structure
	wndClass.lpfnWndProc	= (WNDPROC)WndProc;	// WndProc Message Callback
	wndClass.lpszClassName	= WINDOW_TITLE;		// Window Class Name
	wndClass.hInstance		= g_hInst;			// Handle to the application
	wndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);	// default cursor
	wndClass.hbrBackground	= (HBRUSH)(COLOR_WINDOWFRAME);	// background brush

	// Register a new type of window
	RegisterClassEx(&wndClass);

	// create the window
	g_hWnd = CreateWindow(WINDOW_TITLE,			// window class name
						WINDOW_TITLE,			// window title
						g_bWindowed ? WS_OVERLAPPEDWINDOW | WS_VISIBLE : (WS_POPUP | WS_VISIBLE),	// window style
						CW_USEDEFAULT, CW_USEDEFAULT,	// window position (x,y)
						SCREEN_WIDTH, SCREEN_HEIGHT,	// window resolution (w,h)
						NULL, NULL,						// Parent window and menu
						g_hInst,						// Handle to the application
						NULL);							// NOT USED
	
	// display the window
	ShowWindow(g_hWnd, SW_SHOW);
	UpdateWindow(g_hWnd);
}

// Entry point to the windows application
int WINAPI wWinMain(HINSTANCE hInstance,		// Handle to the app
					HINSTANCE hPrevInstance,	// Handle to previous app
					LPTSTR lpCmdLine,			// command line string
					int lpCmdShow)				// Show window value
{
	g_hInst = hInstance;
	g_bWindowed = true;
	
	// initialize the window
	InitWindow();

	// use this message structure to catch messages in the window
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	// this is where we initialize our game class (runs once)
	SS5Game.Init(g_hWnd, g_hInst, g_bWindowed);

	// initialize our game timer here
	_int64 countsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	float secsPerCount = 1.0f / (float) countsPerSec;

	_int64 prevTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);


	// windows message loop / game loop
	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// calculate the elapse time between frames (timer)
		_int64 currentTimeStamp = 0;
		QueryPerformanceCounter((LARGE_INTEGER*)&currentTimeStamp);
		float dt = (currentTimeStamp - prevTimeStamp) * secsPerCount;

		// call the game's update and render functions
		SS5Game.Update(dt);
		SS5Game.Render();

		// prepare the timer for the next iteration (prev = curr)
		prevTimeStamp = currentTimeStamp;

	}

	// shutdown the game (occurs once)
	SS5Game.Shutdown();

	// Unregister the window
	UnregisterClass(WINDOW_TITLE, g_hInst);

	return 0;
}

// Windows Message Handler
LRESULT CALLBACK WndProc(HWND hWnd,		// Handle to the window
						UINT message,	// Incoming Message
						WPARAM wparam,	// Message info
						LPARAM lparam)	// Message info
{
	wchar_t msg[32];
	// custom handle windows messages
	switch(message)
	{
	case WM_PAINT:
		{
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
	case WM_KEYDOWN:
		{
			switch(wparam)
			{
			/*case VK_ESCAPE:
				{
					PostQuitMessage(0);
					break;
				}*/
			case VK_BACK:
				{
					if(Initializer::GetInstance()->IsNewScoreScreenInView())
						Initializer::GetInstance()->backSpaceName();
				}
			}

				break;
		}
	case WM_CHAR:
		{
			if(Initializer::GetInstance()->IsNewScoreScreenInView())
			{
				if(wparam != VK_BACK)
				{
					swprintf_s(msg, L"%c", (wchar_t)wparam);
					std::wstring temp = msg;
					std::string temp2(temp.begin(), temp.end());
					Initializer::GetInstance()->setUserName(temp2);
				}
			}
		}
	}

	// pass the message to the default message handler
	return DefWindowProc(hWnd, message, wparam, lparam);
}