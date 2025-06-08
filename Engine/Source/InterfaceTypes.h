#pragma once

#ifdef MAIN_FILE
#include "Main/DefaultMain.h"
typedef DefaultMain Main;
#endif // MAIN_FILE

#ifdef WINDOW_FILE
#ifdef PLATFORM_WINDOWS
#include "Window/WindowsWindow.h"
typedef WindowsWindow Window;
#endif

#ifdef PLATFORM_LINUX

#endif
#endif // WINDOW_FILE
