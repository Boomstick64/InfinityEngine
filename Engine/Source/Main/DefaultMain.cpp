#include "DefaultMain.h"

#include WINDOW_FILE

#include <cstdio>


#include "Source/Handles/StackHandle.h"

int DefaultMain::Main()
{
	StackHandle Handle = StackHandle<DefaultWindow>();

	printf("The function is %s \n", __FUNCSIG__);

	IWindow& WindowInterface = Handle.GetObj();

	WindowInterface.Register();

	WindowInterface.Create();

	WindowInterface.Show();

	WindowInterface.Update();

	return 0;
}