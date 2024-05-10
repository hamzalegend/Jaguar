
project "Vendor"
    location "./"
    kind "StaticLib"
    language "c++"
    staticruntime "off"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

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
        "./imgui",
        "./glfw/include",
        "./yaml-cpp/include",
        "%{wks.location}/Vendor/Box2D/include",
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
