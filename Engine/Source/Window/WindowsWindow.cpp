
#include "WindowsWindow.h"

#include <cstdio>

#include RENDERER_FILE

const wchar_t WindowClassName[] = L"Infinity Engine";

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void OnSize(HWND hwnd, UINT flag, int width, int height);

WindowsWindow::WindowsWindow() : ActiveRenderer(new Renderer())
{
	
}

WindowsWindow::~WindowsWindow()
{
	delete ActiveRenderer;
}

void WindowsWindow::Register()
{
	FreeConsole();

	// Register window class
	WNDCLASS WindowClass = { 0 };
	WindowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WindowClass.hInstance = NULL;
	WindowClass.lpfnWndProc = WindowProc;
	WindowClass.lpszClassName = WindowClassName; // Needs to be the same name when creating the window as well
	WindowClass.style = CS_HREDRAW | CS_VREDRAW;

	// Actually register class
	if (!RegisterClass(&WindowClass))
	{
		MessageBox(NULL, L"Could not register class", L"Error", MB_OK);
	}

}

void WindowsWindow::Create()
{
	// Create the window
	WindowHandle = CreateWindowEx(
		0,								// Optional window styles
		WindowClassName,				// Window class
		L"InfinityEngine",				// Window text
		WS_OVERLAPPEDWINDOW,			// Window style

		// Position and size
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,		// Parent window
		NULL,		// Menu
		NULL,	// Instance handle
		NULL		// Additional application data
	);

	if (ActiveRenderer != nullptr)
	{
		ActiveRenderer->SetWindowInterface(this);

		ActiveRenderer->Init();
	}
	
}

void WindowsWindow::Show()
{
	if (WindowHandle == NULL) return;

	ShowWindow(WindowHandle, SW_RESTORE);
}

void WindowsWindow::Update()
{
	// Run the message loop

	RECT Rect = {};
	MSG msg = {};
	bool ApplicationRunning = true;

	while (ApplicationRunning)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				ApplicationRunning = false;
				break;
			}

			if (GetWindowRect(WindowHandle, &Rect))
			{
				Width = Rect.right - Rect.left;
				Height = Rect.bottom - Rect.top;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (ActiveRenderer != nullptr)
		{
			ActiveRenderer->Update();
		}
	}
	
	DeleteObject(WindowHandle);
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
