#include "DefaultMain.h"

#define WINDOW_FILE
#include "../InterfaceTypes.h"
#undef WINDOW_FILE

#include "Source/Handles/StackHandle.h"

int DefaultMain::Main()
{
	StackHandle Handle = StackHandle<Window>();

	IWindow& WindowInterface = Handle.GetObj();

	WindowInterface.Register();

	WindowInterface.Create();

	WindowInterface.Show();

	WindowInterface.Update();

	return 0;
}