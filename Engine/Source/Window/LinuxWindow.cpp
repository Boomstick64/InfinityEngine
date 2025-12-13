#include "LinuxWindow.h"

#ifdef PLATFORM_LINUX

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include <stdio.h>
#include <stdlib.h>

LinuxWindow::LinuxWindow()
{
    
}
LinuxWindow::~LinuxWindow()
{

}

struct coord {
	int x, y;
} dot; 

void LinuxWindow::Init()
{
    dot.x = 100;
	dot.y = 100;
	CurrentDisplay = XOpenDisplay((char *)0);
	Screen = DefaultScreen(CurrentDisplay);
	Black = BlackPixel(CurrentDisplay, Screen);
	White = WhitePixel(CurrentDisplay, Screen);
	Red = RGB(255, 0, 0);
	Blue = RGB(0, 0, 255);
	CurrentWindow = XCreateSimpleWindow(CurrentDisplay, DefaultRootWindow(CurrentDisplay), 0, 0, 300, 300, 5, White, Black);
	XSetStandardProperties(CurrentDisplay, CurrentWindow, "InfinityEngine", "Hello", None, NULL, 0, NULL);
	XSelectInput(CurrentDisplay, CurrentWindow, ExposureMask | ButtonPressMask | KeyPressMask);
	GraphicsContext = XCreateGC(CurrentDisplay, CurrentWindow, 0, 0);
	XSetBackground(CurrentDisplay, GraphicsContext, White);
	XSetForeground(CurrentDisplay, GraphicsContext, Black);
	XClearWindow(CurrentDisplay, CurrentWindow);
	XMapRaised(CurrentDisplay, CurrentWindow);
}

void LinuxWindow::Create()
{

}

void LinuxWindow::Update()
{
    XEvent Event;
	KeySym Key;
	char Text[255];
	int PendingEvent = XPending(CurrentDisplay);
    bool ApplicationRunning = true;

	while (ApplicationRunning)
    {
        XNextEvent(CurrentDisplay, &Event);
		if (XFilterEvent(&Event, None))
		{
			continue;
		}

		if (Event.type == Expose && Event.xexpose.count == 0)
		{
			Draw();
		}
		if (Event.type == KeyPress && XLookupString(&Event.xkey, Text, 255, &Key, 0) == 1)
		{
			if (Text[0] == 'q')
			{
				Close();
                ApplicationRunning = false;
			}
			printf("You pressed the %c key \n", Text[0]);
		}
		if (Event.type == ButtonPress)
		{
			int x = Event.xbutton.x, y = Event.xbutton.y;
			XSetForeground(CurrentDisplay, GraphicsContext, Red);
			XDrawLine(CurrentDisplay, CurrentWindow, GraphicsContext, dot.x, dot.y, x, y);
			XSetForeground(CurrentDisplay, GraphicsContext, Blue);
			strcpy(Text, "Hello World");
			XDrawString(CurrentDisplay, CurrentWindow, GraphicsContext, x, y, Text, strlen(Text));
			dot.x = x; dot.y = y;
		}
    }
}

void LinuxWindow::Draw()
{
    XClearWindow(CurrentDisplay, CurrentWindow);
}

void LinuxWindow::Close()
{
    XFreeGC(CurrentDisplay, GraphicsContext);
	XDestroyWindow(CurrentDisplay, CurrentWindow);
	XCloseDisplay(CurrentDisplay);
	exit(0);
}

unsigned long LinuxWindow::RGB(int r, int g, int b)
{
    return b + (g<<8) + (r<<16);
}

#endif