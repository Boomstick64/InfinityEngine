workspace "InfinityEngine"
	configurations {"Debug", "Release"}
	architecture "x86_64"

	projectname = "InfinityEngine"
	project(projectname)
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++20"

		targetdir "%{wks.location}/Build/Binaries/%{cfg.architecture}-%{cfg.buildcfg}/"
		objdir "%{wks.location}/Build/Intermediate/%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}/"

		location "%{wks.location}/Intermediate/ProjectFiles/"
		files { 
			"%{prj.location}/**.h", "%{prj.location}/**.hpp", "%{prj.location}/**.hh", "%{prj.location}/**.hxx",
			"%{prj.location}/**.c", "%{prj.location}/**.cpp", "%{prj.location}/**.cc", "%{prj.location}/**.cxx",
			"%{prj.location}/**.lua", "%{prj.location}/**.txt", "%{prj.location}/**.md", "%{prj.location}/**.ini"
		}

		filter "configurations:Debug"
			defines { "DEBUG", "_DEBUG" }
			symbols "On"
		filter{}

		filter "configurations:Release"
			defines { "NDEBUG" }
			optimize "On"
		filter{}