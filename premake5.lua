function NewProject(projectname)
	project(projectname)
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	
	targetdir "%{wks.location}/Build/Binaries/%{cfg.architecture}-%{cfg.buildcfg}/"
	objdir "%{wks.location}/Build/Intermediate/%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}/"
	
	location "%{wks.location}/Intermediate/ProjectFiles/"
	files { 
		"%{wks.location}/**.h", "%{wks.location}/**.hpp", "%{wks.location}/**.hh", "%{wks.location}/**.hxx",
		"%{wks.location}/**.c", "%{wks.location}/**.cpp", "%{wks.location}/**.cc", "%{wks.location}/**.cxx",
		"%{wks.location}/**.lua", "%{wks.location}/**.txt", "%{wks.location}/**.md", "%{wks.location}/**.ini"
	}
	
	includedirs {"{wks.location}", "{prj.location}", "{wks.location}/Source/" }

	filter "configurations:Debug"
		defines { "DEBUG", "_DEBUG" }
		symbols "On"
	filter{}
	
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
	filter{}
end

function SetSharedLib()
	kind "SharedLib"
	defines {"%{prj.name}_BUILD_DLL"}
end

workspace "InfinityEngine"
	configurations {"Debug", "Release"}
	architecture "x86_64"

	matches = os.matchfiles("./**/**.lua")
	for i in ipairs(matches) do
	matchedpath = "./" .. path.getrelative("./", matches[i])
	print(matchedpath)
	include(matchedpath)
	end

