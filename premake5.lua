function NewProject(projectname, projectdirectory)
	if (projectdirectory == nil) then
		projectdirectory = projectname
	end
	project(projectname)
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	
	targetdir "%{wks.location}/Binaries/%{cfg.architecture}-%{cfg.buildcfg}/"
	objdir "%{wks.location}/Binaries/Intermediate/%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}/"

	location "%{wks.location}/Intermediate/ProjectFiles/%{prj.name}/"
	files { 
		"%{wks.location}/" .. projectdirectory .. "/**.h", "%{wks.location}/" .. projectdirectory .. "/**.hpp", "%{wks.location}/" .. projectdirectory .. "/**.hh", "%{wks.location}/" .. projectdirectory .. "/**.hxx",
		"%{wks.location}/" .. projectdirectory .. "/**.c", "%{wks.location}/" .. projectdirectory .. "/**.cpp", "%{wks.location}/" .. projectdirectory .. "/**.cc", "%{wks.location}/" .. projectdirectory .. "/**.cxx",
		"%{wks.location}/" .. projectdirectory .. "/**.lua", "%{wks.location}/" .. projectdirectory .. "/**.txt", "%{wks.location}/" .. projectdirectory .. "/**.md", "%{wks.location}/" .. projectdirectory .. "/**.ini"
	}

	includedirs {"%{wks.location}", "%{prj.location}", "%{wks.location}/" .. projectdirectory .. "/" }

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
	defines { "%{prj.name:upper(prj.name)}_BUILD_DLL"}
end

workspace "InfinityEngine"
	configurations {"Debug", "Release"}
	architecture "x86_64"

	matches = os.matchfiles("./**.lua")
	for i in ipairs(matches) do
	if not (matches[i] == "premake5.lua") then
		matchedpath = "./" .. path.getrelative("./", matches[i])
		print(matchedpath)
		include(matchedpath)
	end
	
	end

