#ifdef PLATFORM_WINDOWS
#include <windows.h>
#include "Source/Handles/StackHandle.h"
#include "Source/DefaultMain.h"

typedef DefaultMain Main;

int main()
{
	StackHandle Handle = StackHandle<Main>();

	IMain& Main = Handle.GetObj();

	return Main.Main();
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void OnSize(HWND hwnd, UINT flag, int width, int height);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR lpCmdLine, _In_ int nCmdShow)
{
	// Register the window class
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// Create the window

	HWND hwnd = CreateWindowEx(
		0,								// Optional window styles
		CLASS_NAME,						// Window class
		L"InfinityEngine",				// Window text
		WS_OVERLAPPEDWINDOW,			// Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,		// Parent window
		NULL,		// Menu
		hInstance,	// Instance handle
		NULL		// Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	// Run the message loop

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			// All painting occurs here, between BeginPaint and EndPaint

			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

			EndPaint(hwnd, &ps);
		}
	case WM_SIZE:
		{
			int width = LOWORD(lParam); // Macro to get the low-order word
			int height = HIWORD(lParam);// Macro to get the high-order word

			// Respond to the message:
			OnSize(hwnd, (UINT)wParam, width, height);
		}
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void OnSize(HWND hwnd, UINT flag, int width, int height)
{

}
#endif

#ifdef PLATFORM_LINUX

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include <stdio.h>
#include <stdlib.h>

Display *dis;
int screen;
Window win;
GC gc;
unsigned long black, white, red, blue;

void Init();
void Close();
void Draw();
unsigned long RGB(int r, int g, int b);

struct coord {
	int x, y;
} dot; 

int main()
{
	Init();
	XEvent event;
	KeySym key;
	char text[255];
	int pendingevent = XPending(dis);
	while (1)
	{
		XNextEvent(dis, &event);
		if (XFilterEvent(&event, None))
		{
			continue;
		}

		if (event.type == Expose && event.xexpose.count == 0)
		{
			Draw();
		}
		if (event.type == KeyPress && XLookupString(&event.xkey, text, 255, &key, 0) == 1)
		{
			if (text[0] == 'q')
			{
				Close();
			}
			printf("You pressed the %c key \n", text[0]);
		}
		if (event.type == ButtonPress)
		{
			int x = event.xbutton.x, y = event.xbutton.y;
			XSetForeground(dis, gc, red);
			XDrawLine(dis, win, gc, dot.x, dot.y, x, y);
			XSetForeground(dis, gc, blue);
			strcpy(text, "Hello World");
			XDrawString(dis, win, gc, x, y, text, strlen(text));
			dot.x = x; dot.y = y;
		}
	}
	return 0;
}

void Init()
{
	dot.x = 100;
	dot.y = 100;
	dis = XOpenDisplay((char *)0);
	screen = DefaultScreen(dis);
	black = BlackPixel(dis, screen);
	white = WhitePixel(dis, screen);
	red = RGB(255, 0, 0);
	blue = RGB(0, 0, 255);
	win = XCreateSimpleWindow(dis, DefaultRootWindow(dis), 0, 0, 300, 300, 5, white, black);
	XSetStandardProperties(dis, win, "InfinityEngine", "Hello", None, NULL, 0, NULL);
	XSelectInput(dis, win, ExposureMask | ButtonPressMask | KeyPressMask);
	gc = XCreateGC(dis, win, 0, 0);
	XSetBackground(dis, gc, white);
	XSetForeground(dis, gc, black);
	XClearWindow(dis, win);
	XMapRaised(dis, win);
}

void Close()
{
	XFreeGC(dis, gc);
	XDestroyWindow(dis, win);
	XCloseDisplay(dis);
	exit(0);
}

void Draw()
{
	XClearWindow(dis, win);
}
unsigned long RGB(int r, int g, int b)
{
	return b + (g<<8) + (r<<16);
}

#endif