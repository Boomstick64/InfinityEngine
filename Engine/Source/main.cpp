#include MAIN_FILE

#include "Source/Handles/StackHandle.h"

int main()
{
	StackHandle Handle = StackHandle<Main>();

	IMain& Main = Handle.GetObj();
	
	return Main.Main();
}