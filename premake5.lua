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

   SDL_DIR = "vendor/SDL"
   RECAST_NAV = "RobinTheEngine/vendor/recastnavigation"
   game_includes = {
	   RECAST_NAV .. "/Detour/Include",
       RECAST_NAV .. "/DetourCrowd/Include",
       RECAST_NAV .. "/Recast/Include",
	   RECAST_NAV .. "/DetourTileCache/Include",
       SDL_DIR .. "/include", 
   }

   game_files = {
       RECAST_NAV .. "/Detour/Include/*.h", 
       RECAST_NAV .. "/Detour/Source/*.cpp",
	   RECAST_NAV .. "/DetourCrowd/Include/*.h",
       RECAST_NAV .. "/DetourCrowd/Source/*.cpp",
	   RECAST_NAV .. "/DetourTileCache/Include/*.h",
       RECAST_NAV .. "/DetourTileCache/Source/*.cpp",
   }

   engine_includes = {
        RECAST_NAV .. "/DebugUtils/Include",
        RECAST_NAV .. "/DetourTileCache/Include",
        RECAST_NAV .. "/Recast/Include",
	    table.unpack(game_includes),
   }

   engine_files = {
        RECAST_NAV .. "/DebugUtils/Include/*.h",
        RECAST_NAV .. "/DebugUtils/Source/*.cpp",
        RECAST_NAV .. "/Recast/Include/*.h",
        RECAST_NAV .. "/Recast/Source/*.cpp",
        table.unpack(game_files),
   }

    group "Dependencies"

    project "DebugUtils"
	    language "C++"
	    kind "StaticLib"
        location "RecastDeps"
	    includedirs { 
		    RECAST_NAV .. "/DebugUtils/Include",
		    RECAST_NAV .. "/Detour/Include",
		    RECAST_NAV .. "/DetourTileCache/Include",
		    RECAST_NAV .. "/Recast/Include"
	    }
	    files {
		    RECAST_NAV .. "/DebugUtils/Include/*.h",
		    RECAST_NAV .. "/DebugUtils/Source/*.cpp"
	    }

    project "Detour"
	    language "C++"
	    kind "StaticLib"
        location "RecastDeps"
	    includedirs { 
		    RECAST_NAV .. "/Detour/Include" 
	    }
	    files { 
		    RECAST_NAV .. "/Detour/Include/*.h", 
		    RECAST_NAV .. "/Detour/Source/*.cpp" 
	    }

    project "DetourCrowd"
	    language "C++"
	    kind "StaticLib"
	    location "RecastDeps"
        includedirs {
		    RECAST_NAV .. "/DetourCrowd/Include",
		    RECAST_NAV .. "/Detour/Include",
		    RECAST_NAV .. "/Recast/Include"
	    }
	    files {
		    RECAST_NAV .. "/DetourCrowd/Include/*.h",
		    RECAST_NAV .. "/DetourCrowd/Source/*.cpp"
	    }

    project "DetourTileCache"
	    language "C++"
	    kind "StaticLib"
	    location "RecastDeps"
        includedirs {
		    RECAST_NAV .. "/DetourTileCache/Include",
		    RECAST_NAV .. "/Detour/Include",
		    RECAST_NAV .. "/Recast/Include"
	    }
	    files {
		    RECAST_NAV .. "/DetourTileCache/Include/*.h",
		    RECAST_NAV .. "/DetourTileCache/Source/*.cpp"
	    }

    project "Recast"
	    language "C++"
	    kind "StaticLib"
	    location "RecastDeps"
        includedirs { 
		    RECAST_NAV .. "/Recast/Include" 
	    }
	    files { 
		    RECAST_NAV .. "/Recast/Include/*.h",
		    RECAST_NAV .. "/Recast/Source/*.cpp" 
	    }

    group ""

    project "NavBuilder"
	    language "C++"
	    kind "WindowedApp"
        location "NavBuilder"
        cppdialect "C++20"
	    includedirs {
		    RECAST_NAV .. "/RecastDemo/Include",
		    RECAST_NAV .. "/RecastDemo/Contrib",
		    RECAST_NAV .. "/RecastDemo/Contrib/fastlz",
		    RECAST_NAV .. "/DebugUtils/Include",
		    RECAST_NAV .. "/Detour/Include",
		    RECAST_NAV .. "/DetourCrowd/Include",
		    RECAST_NAV .. "/DetourTileCache/Include",
		    RECAST_NAV .. "/Recast/Include",
            SDL_DIR .. "/include",
	    }
	    files	{ 
		    RECAST_NAV .. "/RecastDemo/Include/*.h",
		    RECAST_NAV .. "/RecastDemo/Source/*.cpp",
		    RECAST_NAV .. "/RecastDemo/Contrib/fastlz/*.h",
		    RECAST_NAV .. "/RecastDemo/Contrib/fastlz/*.c"
	    }
        debugdir ("bin/" .. outputdir .. "/%{prj.name}")
        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	    -- project dependencies
	    links { 
		    "DebugUtils",
		    "Detour",
		    "DetourCrowd",
		    "DetourTileCache",
		    "Recast"
	    }

	    -- windows library cflags and libs
	    configuration { "windows" }
		    includedirs { RECAST_NAV .. "/RecastDemo/Contrib/SDL/include" }
		    libdirs { SDL_DIR .. "/lib/%{cfg.architecture:gsub('x86_64', 'x64')}" }
		    links { 
			    "glu32",
			    "opengl32",
			    "SDL2",
			    "SDL2main",
		    }
            defines { "WIN32", "_WINDOWS", "_CRT_SECURE_NO_WARNINGS", "_HAS_EXCEPTIONS=0" }
		    postbuildcommands {
			    -- Copy the SDL2 dll to the Bin folder.
			    '{COPY} "%{path.getabsolute(SDL_DIR .. "/lib/" .. cfg.architecture:gsub("x86_64", "x64") .. "/SDL2.dll")}" "%{cfg.targetdir}"',
			    '{COPY} "%{path.getabsolute(RECAST_NAV .. "/RecastDemo/bin/")}" "%{cfg.targetdir}"',
		    }

   project "RobinTheEngine"
      location "RobinTheEngine"
      kind "StaticLib"
      language "C++"
	  cppdialect "C++20"
	  staticruntime "on"
	  libdirs {"vendor/lua/lib/", }

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
			"GLFW_INCLUDE_NONE",
            "WIN32",
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

   project "Sandbox"
      location "Sandbox"
      kind "ConsoleApp"
      language "C++"
	  cppdialect "C++20"
	  staticruntime "on"
      defines {"NOMINMAX"}
      
      td = "bin/" .. outputdir .. "/%{prj.name}"
      targetdir (td)
      objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
      postbuildcommands('{COPYFILE} "$(SolutionDir)vendor/lua/lib/lua54.dll" ' .. '"$(SolutionDir)' .. td .. '"')

      files
      {
         "%{prj.name}/src/**.h",
         "%{prj.name}/src/**.cpp",
         table.unpack(game_files),
      }

      includedirs
      {
         "RobinTheEngine/vendor/spdlog/include",
         "RobinTheEngine/src",
		 "RobinTheEngine/vendor",
		 "RobinTheEngine/vendor/assimp/include",
		 "%{IncludeDir.ViennaGameJobSystem}/include",
		 "%{IncludeDir.entt}",
		 "%{IncludeDir.sol2}",
		 "%{IncludeDir.lua}",
         table.unpack(game_includes),
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

   project "RTEditor"
      location "RTEditor"
      kind "ConsoleApp"
      language "C++"
	  cppdialect "C++20"
	  staticruntime "on"
      defines {"NOMINMAX"}
      
      td = "bin/" .. outputdir .. "/%{prj.name}"
      targetdir (td)
      objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
      postbuildcommands('{COPYFILE} "$(SolutionDir)vendor/lua/lib/lua54.dll" ' .. '"$(SolutionDir)' .. td .. '"')

      files
      {
         "%{prj.name}/src/**.h",
         "%{prj.name}/src/**.cpp",
         table.unpack(engine_files),
      }

      includedirs
      {
         "RobinTheEngine/vendor/spdlog/include",
         "RobinTheEngine/src",
		 "RobinTheEngine/vendor",
		 "RobinTheEngine/vendor/assimp/include",
		 "%{IncludeDir.ViennaGameJobSystem}/include",
		 "%{IncludeDir.entt}",
		 "%{IncludeDir.sol2}",
		 "%{IncludeDir.lua}",
         table.unpack(engine_includes),
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

