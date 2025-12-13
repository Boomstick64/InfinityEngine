#pragma once

#ifdef PLATFORM_LINUX

#include "../Interfaces/WindowInterface.h"

#include <X11/Xlib.h>

class LinuxWindow : public IWindow
{
public:
    LinuxWindow();
    ~LinuxWindow();

    virtual void Init() override;
    virtual void Create() override;
    virtual void Show() override {}
    virtual void Update() override;
    virtual void Close() override;

    virtual void Draw();
    unsigned long RGB(int r, int g, int b);

protected:
    Display *CurrentDisplay;
    int Screen;
    Window CurrentWindow;
    GC GraphicsContext;
    unsigned long Black, White, Red, Blue;

public:
    virtual int GetWindowWidth() override { return 0; }
	virtual int GetWindowHeight() override { return 0; }
};

#endif