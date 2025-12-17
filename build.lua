workspace "Copium"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "client"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "library"
	include "library/build.lua"
group ""

include "client/build.lua"