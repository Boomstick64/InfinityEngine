NewProject("InfinityEngine", "Engine")
	defines {
		"MAIN_FILE=\"Source/Main/DefaultMain.h\"", "MAIN_TYPE=DefaultMain",
	}
	filter "system:Windows"
		libdirs {os.findlib("d3d12"), os.findlib("dxgi"), os.findlib("d3dcompiler")}
		links {"d3d12", "dxgi", "d3dcompiler"}
	defines {
		"WINDOW_FILE=\"Source/Window/WindowsWindow.h\"", "WINDOW_TYPE=WindowsWindow",
		"RENDERER_FILE=\"Source/Graphics/DirectX.h\"", "RENDERER_TYPE=DirectX"
	}
	filter {}

	filter "system:Linux"
		libdirs {os.findlib("X11")}
		links {"X11"}
		defines {
			"WINDOW_FILE=\"Source/Window/LinuxWindow.h\"", "WINDOW_TYPE=LinuxWindow"
	}
	filter {}