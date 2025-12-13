#pragma once

#include "../Interfaces/WindowInterface.h"

#include <windows.h>
#include <iostream>
#include <memory.h>

class IRenderer;

class WindowsWindow : public IWindow
{
public:

	WindowsWindow();
	~WindowsWindow();
	virtual void Register() override;
	virtual void Create() override;
	virtual void Show() override;
	virtual void Update() override;

	HWND WindowHandle;

protected:

	int Width = 0;
	int Height = 0;

	IRenderer* ActiveRenderer = nullptr;

public:

	virtual int GetWindowWidth() override { return Width; }
	virtual int GetWindowHeight() override { return Height; }
};