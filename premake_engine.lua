   workspace "RobinTheEngine"
      architecture "x64"
      startproject "RobinTheEngine"
   
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
   IncludeDir["yaml"] = "RobinTheEngine/vendor/yaml-cpp/include"
   IncludeDir["sol2"] = "RobinTheEngine/vendor/sol2/include"
   IncludeDir["lua"] = "vendor/lua/include"

   group "Dependencies"
   include "RobinTheEngine/vendor/GLFW"
   include "RobinTheEngine/vendor/imgui"
   --include "RobinTheEngine/vendor/DirectXTK12"
   include "RobinTheEngine/vendor/DirectXTK"
   include "RobinTheEngine/vendor/assimp"
   include "RobinTheEngine/vendor/yaml-cpp"
   group ""

   project "RobinTheEngine"
      location "RobinTheEngine"
      kind "WindowedApp"
      language "C++"
      cppdialect "C++20"
      staticruntime "on"
      libdirs {"vendor/lua/lib/", }
      defines "RTE_BUILD_ENGINE"
      td = "bin/" .. outputdir .. "/%{prj.name}"
      targetdir (td)
      objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
      postbuildcommands('{COPYFILE} "$(SolutionDir)vendor/lua/lib/lua54.dll" ' .. '"$(SolutionDir)' .. td .. '"')

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
         "GLFW_INCLUDE_NONE",
         "SOL_ALL_SAFETIES_ON",
         "NOMINMAX"
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
         "%{IncludeDir.yaml}",
         --"%{IncludeDir.DirectXTK12}/Inc",
         "%{IncludeDir.DirectXTK}/Inc",
         "%{IncludeDir.assimp}/include",
         "%{IncludeDir.ViennaGameJobSystem}/include",
         "%{IncludeDir.sol2}",
         "%{IncludeDir.lua}",
      }
      
      links
      {
         "GLFW",
         "ImGui",
         "assimp",
         "DirectXTK",
         "yaml-cpp",
         "liblua54",
      }
      
      filter "system:windows"
         systemversion "latest"

         buildoptions {
             "/await",
             "/bigobj" -- need only for debug build
          }

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

      filter "configurations:Release"
         defines "RTE_RELEASE"
         optimize "on"
         runtime "Release"

      filter "configurations:Dist"
         defines "RTE_DIST"
         optimize "on"
         runtime "Release"
