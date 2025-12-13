#pragma once

#ifdef PLATFORM_WINDOWS

#include "../Interfaces/WindowInterface.h"

#include <windows.h>

class IRenderer;

class WindowsWindow : public IWindow
{
public:

	WindowsWindow();
	~WindowsWindow();
	virtual void Init() override;
	virtual void Create() override;
	virtual void Show() override;
	virtual void Update() override;
	virtual void Close() override {}

	HWND WindowHandle;

protected:

	int Width = 0;
	int Height = 0;

	IRenderer* ActiveRenderer = nullptr;

public:

	virtual int GetWindowWidth() override { return Width; }
	virtual int GetWindowHeight() override { return Height; }
};

#endif