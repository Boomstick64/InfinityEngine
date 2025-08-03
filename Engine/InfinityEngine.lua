NewProject("InfinityEngine", "Engine")
	defines {
		"MAIN_FILE=\"Source/Main/DefaultMain.h\"", "MAIN_TYPE=DefaultMain",
	}
	filter "system:Windows"
	defines {
		"WINDOW_FILE=\"Source/Window/WindowsWindow.h\"", "WINDOW_TYPE=WindowsWindow"
	}
	filter {}