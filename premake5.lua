   workspace "RobinTheEngine"
      architecture "x64"
      startproject "Sandbox"
   
      configurations
      {
         "Debug",
         "Release",
         "Dist"
      }


   outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
 
   -- Include directories relative to root folder (solution directory)
   IncludeDir = {}
   IncludeDir["GLFW"] = "RobinTheEngine/vendor/GLFW/include"
   IncludeDir["ImGui"] = "RobinTheEngine/vendor/imgui"
   --IncludeDir["DirectXTK12"] = "RobinTheEngine/vendor/DirectXTK12"
   IncludeDir["DirectXTK"] = "RobinTheEngine/vendor/DirectXTK"
   IncludeDir["assimp"] = "RobinTheEngine/vendor/assimp"
   IncludeDir["ViennaGameJobSystem"] = "RobinTheEngine/vendor/ViennaGameJobSystem"
   IncludeDir["entt"] = "RobinTheEngine/vendor/entt/single_include/entt"

   group "Dependencies"
   include "RobinTheEngine/vendor/GLFW"
   include "RobinTheEngine/vendor/imgui"
   --include "RobinTheEngine/vendor/DirectXTK12"
   include "RobinTheEngine/vendor/DirectXTK"
   include "RobinTheEngine/vendor/assimp"
   group ""


   project "RobinTheEngine"
      location "RobinTheEngine"
      kind "StaticLib"
      language "C++"
	  cppdialect "C++20"
	  staticruntime "on"
	  libdirs { "%{prj.name}/../vendor/bin/DirectXTK/Debug/" }
      defines {"NOMINMAX"}

      targetdir ("bin/" .. outputdir .. "/%{prj.name}")
      objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

      pchheader "rtepch.h"
      pchsource "RobinTheEngine/src/rtepch.cpp"

      files
      {
         "%{prj.name}/src/**.h",
         "%{prj.name}/src/**.cpp"
      }
	  defines
	  {
	     "_CRT_SECURE_NO_WARNINGS",
		 "GLFW_INCLUDE_NONE"
	  }

      includedirs
      {
         "%{prj.name}/src",
         "%{prj.name}/vendor/spdlog/include",
		 "%{prj.name}/vendor/stb",
         "%{prj.name}/vendor/DirectX12-Headers/include/directx",
         "%{IncludeDir.GLFW}",
		 "%{IncludeDir.ImGui}",
		 "%{IncludeDir.entt}",
		 --"%{IncludeDir.DirectXTK12}/Inc",
		 "%{IncludeDir.DirectXTK}/Inc",
		 "%{IncludeDir.assimp}/include",
		 "%{IncludeDir.ViennaGameJobSystem}/include",

      }
      
      links
      {
         "GLFW",
		 "opengl32.lib",
		 "ImGui",
		 "assimp",
		 "DirectXTK"

      }
	  


      filter "system:windows"
         systemversion "latest"

         buildoptions { "/await"}

         defines
         {
            "RTE_PLATFORM_WINDOWS",
            "RTE_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
         }


      filter "configurations:Debug"
         defines "RTE_DEBUG"
         symbols "on"
		 runtime "Debug"
		 libdirs { "vendor/bin/DirectXTK/Debug/" }

      filter "configurations:Release"
         defines "RTE_RELEASE"
         optimize "on"
		 runtime "Release"
		 libdirs { "vendor/bin/DirectXTK/Release/" }

      filter "configurations:Dist"
         defines "RTE_DIST"
         optimize "on"
		 runtime "Release"
		 libdirs { "vendor/bin/DirectXTK/Release/" }


   project "Sandbox"
      location "Sandbox"
      kind "ConsoleApp"
      language "C++"
	  cppdialect "C++20"
	  staticruntime "on"
      defines {"NOMINMAX"}

      targetdir ("bin/" .. outputdir .. "/%{prj.name}")
      objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

      files
      {
         "%{prj.name}/src/**.h",
         "%{prj.name}/src/**.cpp"
      }

      includedirs
      {
         "RobinTheEngine/vendor/spdlog/include",
         "RobinTheEngine/src",
		 "RobinTheEngine/vendor",
		 "RobinTheEngine/vendor/assimp/include",
		 "%{IncludeDir.ViennaGameJobSystem}/include",
		 "%{IncludeDir.entt}",
      }

      links
      {
         "RobinTheEngine"
      }

      filter "system:windows"

         systemversion "latest"

         buildoptions { "/await"}

         defines
         {
            "RTE_PLATFORM_WINDOWS"
         }

      filter "configurations:Debug"
         defines "RTE_DEBUG"
         symbols "on"
		 runtime "Debug"

      filter "configurations:Release"
         defines "RTE_RELEASE"
         optimize "on"
		 runtime "Release"

      filter "configurations:Dist"
         defines "RTE_DIST"
         optimize "on"
		 runtime "Release"






