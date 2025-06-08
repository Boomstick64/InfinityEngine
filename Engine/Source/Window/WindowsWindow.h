#pragma once

#include "../Interfaces/WindowInterface.h"

#include <windows.h>

class WindowsWindow : public IWindow
{
public:

	virtual void Register() override;
	virtual void Create() override;
	virtual void Show() override;
	virtual void Update() override;

protected:

	HWND WindowHandle;
};