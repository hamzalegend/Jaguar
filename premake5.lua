workspace "Jaguar"

    location "./"
    architecture "x64"

    startproject "Editor"

    configurations
    {
        "Debug",
        "Release",
        "Dist",
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
group "Core"
project "Jaguar"
    location "Jaguar"
    kind "SharedLib"
    language "c++"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }
    includedirs
    {
        "%{wks.location}/glad/include/",
        "%{wks.location}/Vendor/glfw/include",
        "%{wks.location}/Vendor/glm/include",
        "%{prj.name}/vendor",
        "Jaguar/src",
        "%{wks.location}/Vendor",
        "%{wks.location}/Vendor/yaml-cpp/include",
        "%{wks.location}/Vendor/Box2D/include",
        "C:/Program Files/Assimp/include",
        "%{wks.location}/ImGui",
        "%{wks.location}/Vendor/mono/include",
    }

    libdirs
    {
        "%{wks.location}/Vendor/glfw/lib",
        "%{wks.location}/Vendor/yaml-cpp/build/Debug",
        "C:/Program Files/Assimp/lib/x64",
        "C:/Program Files/Assimp/bin/x64",
        "%{wks.location}/Vendor/mono/lib/Debug",
    }

    links
    {
        -- refs
        "glad",
        "Vendor",
        "ImGui",
        "Jaguar-ScriptCore",

        -- external
        "glfw3.dll",
        "glfw3dll.lib",
        "assimp-vc143-mt.lib",
        "yaml-cppd.lib",
        "libmono-static-sgen.lib",
        "Ws2_32.lib",
        "Winmm.lib",
        "Version.lib",
        "Bcrypt.lib",
    }

    filter "system:windows"
        cppdialect "c++17"
        -- staticbuildoptions  "on"
        systemversion "latest"

        defines
        {
            "_DEBUG",
            "_CONSOLE",
            "JR_PLATFORM_WINDOWS",
            "JR_BUILD_DLL",
        }

        postbuildcommands {
            "{COPY} %{wks.location}/bin/Debug-windows-x86_64/Jaguar/Jaguar.dll %{wks.location}/bin/Debug-windows-x86_64/SandBox",
            "{COPY} %{wks.location}/bin/Debug-windows-x86_64/Jaguar/Jaguar.dll %{wks.location}/bin/Debug-windows-x86_64/Editor",
          }

        filter "configurations:Debug"
            defines "JR_DEBUG"
            symbols "On"
        
        filter "configurations:Release"
            defines "JR_Release"
            optimize "On"
        
        filter "configurations:Dist"
            defines "JR_Dist"
            optimize "On"

project "Jaguar-ScriptCore"
    location "Jaguar-ScriptCore"
    kind "SharedLib"
    language "C#"
    dotnetframework "4.7.2"
    -- staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.cs",
        "%{prj.name}/src/**.cs",
    }

    postbuildcommands {
        "{COPY} %{wks.location}/bin/" .. outputdir .. "/%{prj.name}/Jaguar-ScriptCore.dll %{wks.location}/Editor/res/Scripts",
        "{COPY} %{wks.location}/bin/" .. outputdir .. "/%{prj.name}/Jaguar-ScriptCore.dll %{wks.location}/SandBox/res/Scripts",
      }

    filter "system:windows"
        filter "configurations:Debug"
            defines "JR_DEBUG"
            symbols "On"
        
        filter "configurations:Release"
            defines "JR_Release"
            optimize "On"
        
        filter "configurations:Dist"
            defines "JR_Dist"
            optimize "On"
group ""

group ""
project "SandBox"
    location "SandBox"
    kind "ConsoleApp"
    language "c++"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }
    includedirs
    {
        "%{wks.location}/glad/include/",
        "%{wks.location}/Vendor/glfw/include",
        "%{wks.location}/Vendor/glm/include",
        "C://Program Files/Assimp/include",
        "%{wks.location}/Jaguar/vendor",
        "%{wks.location}/Jaguar/src",
        "%{wks.location}/Vendor",
        "%{wks.location}/Vendor/yaml-cpp/include",
        "%{wks.location}/Vendor/yaml-cpp/",
        "%{wks.location}/Vendor/Box2D/include",
        "%{wks.location}/ImGui",
        "%{wks.location}/",
    }

    libdirs
    {
        "%{wks.location}/Vendor/glfw/lib",
        "%{wks.location}/Vendor/yaml-cpp/build/Debug",
        "C:/Program Files/Assimp/lib/x64",
        "C:/Program Files/Assimp/bin/x64",
    }

    links
    {
        -- refs
        "Jaguar",
        "glad",
        "ImGui",
        "Vendor",

        -- external
        "glfw3.dll",
        "glfw3dll.lib",
        "assimp-vc143-mt.lib",
        "yaml-cppd.lib",
    }

    filter "system:windows"
        cppdialect "c++17"
        -- staticbuildoptions  "on"
        systemversion "latest"

        defines
        {
            "_DEBUG",
            "_CONSOLE",
            "JR_PLATFORM_WINDOWS",
        }

        filter "configurations:Debug"
            defines "JR_DEBUG"
            symbols "On"
        
        filter "configurations:Release"
            defines "JR_Release"
            optimize "On"
        
        filter "configurations:Dist"
            defines "JR_Dist"
            optimize "On"

group "Tools"
project "Editor"
    location "Editor"
    kind "ConsoleApp"
    language "c++"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }
    includedirs
    {
        "%{wks.location}/glad/include/",
        "%{wks.location}/Vendor/glfw/include",
        "%{wks.location}/Vendor/glm/include",
        "C://Program Files/Assimp/include",
        "%{wks.location}/Jaguar/vendor",
        "%{wks.location}/Jaguar/src",
        "%{wks.location}/Vendor",
        "%{wks.location}/Vendor/yaml-cpp/include",
        "%{wks.location}/Vendor/yaml-cpp/",
        "%{wks.location}/Vendor/Box2D/include",
        "%{wks.location}/ImGui",
        "%{wks.location}/",
    }

    libdirs
    {
        "%{wks.location}/Vendor/glfw/lib",
        "%{wks.location}/Vendor/yaml-cpp/build/Debug",
        "C:/Program Files/Assimp/lib/x64",
        "C:/Program Files/Assimp/bin/x64",
    }

    links
    {
        -- refs
        "Jaguar",
        "glad",
        "ImGui",
        "Vendor",

        -- external
        "glfw3.dll",
        "glfw3dll.lib",
        "assimp-vc143-mt.lib",
        "yaml-cppd.lib",
    }

    filter "system:windows"
        cppdialect "c++17"
        -- staticbuildoptions  "on"
        systemversion "latest"

        defines
        {
            "_DEBUG",
            "_CONSOLE",
            "JR_PLATFORM_WINDOWS",
        }

        filter "configurations:Debug"
            defines "JR_DEBUG"
            symbols "On"
        
        filter "configurations:Release"
            defines "JR_Release"
            optimize "On"
        
        filter "configurations:Dist"
            defines "JR_Dist"
            optimize "On"
group ""

    
group "Dependencies"
project "Vendor"
    location "Vendor"
    kind "StaticLib"
    language "c++"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{wks.location}/Vendor/Box2D/src/**.h",
        "%{wks.location}/Vendor/Box2D/src/**.cpp",
        "%{wks.location}/Vendor/Box2D/include/**.h",
        "%{wks.location}/Vendor/Box2D/include/**.cpp",
    
        -- "%{wks.location}/Vendor/imgui/**.h",
        -- "%{wks.location}/Vendor/imgui/**.cpp",
        -- "%{wks.location}/Vendor/imGuizmo/imGuizmo.h",
        -- "%{wks.location}/Vendor/imGuizmo/imGuizmo.cpp",

        "%{wks.location}/Vendor/STBI/**.*/",
        -- "%{wks.location}/Vendor/yaml-cpp/src/**.h",
        -- "%{wks.location}/Vendor/yaml-cpp/src/**.cpp",
        "%{wks.location}/Vendor/yaml-cpp/include/**.h",
        "%{wks.location}/Vendor/yaml-cpp/include/**.cpp",
    }
    includedirs
    {
        "%{prj.name}/imgui",
        "%{prj.name}/glfw/include",
        "%{prj.name}/yaml-cpp/include",
        "%{prj.name}/Box2D/include",
        "%{wks.location}/ImGui",
    }

    links
    {
        -- "Jaguar",
        -- "yaml-cppd.lib"

    }

    filter "system:windows"
        cppdialect "c++17"
        -- staticbuildoptions  "on"
        systemversion "latest"

        defines
        {
        }

        filter "configurations:Debug"
            defines "JR_DEBUG"
            symbols "On"

        filter "configurations:Release"
            defines "JR_Release"
            optimize "On"

        filter "configurations:Dist"
            defines "JR_Dist"
            optimize "On"

    
project "glad"
    location "glad"
    kind "StaticLib"
    language "c++"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.c",
    }
    includedirs
    {
        "%{prj.name}/include/",
    }

    links
    {
    }

    filter "system:windows"
        cppdialect "c++17"
        -- staticbuildoptions  "on"
        systemversion "latest"

        defines
        {
        }

        filter "configurations:Debug"
            defines "JR_DEBUG"
            symbols "On"

        filter "configurations:Release"
            defines "JR_Release"
            optimize "On"

        filter "configurations:Dist"
            defines "JR_Dist"
            optimize "On"

project "ImGui"
    location "ImGui"
    kind "StaticLib"
    language "c++"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
    }
    includedirs
    {
        "%{prj.name}",
        "%{prj.name}/imgui",
        "%{prj.name}/ImGuizmo",
        "%{wks.location}/Vendor/glfw/include",
        "%{wks.location}/Vendor/glm/include",
        "%{wks.location}/Jaguar/src",
        "%{wks.location}/glad/include/",
    }

    libdirs
    {
        "%{wks.location}/Vendor/glfw/lib",
    }

    links
    {
        "glfw3.dll",
        "glfw3dll.lib",
        "glad",
    }

    filter "system:windows"
        cppdialect "c++17"
        -- staticbuildoptions  "on"
        systemversion "latest"

        defines
        {
            "JR_PLATFORM_WINDOWS",
        }

        filter "configurations:Debug"
            defines "JR_DEBUG"
            symbols "On"

        filter "configurations:Release"
            defines "JR_Release"
            optimize "On"

        filter "configurations:Dist"
            defines "JR_Dist"
            optimize "On"

group ""