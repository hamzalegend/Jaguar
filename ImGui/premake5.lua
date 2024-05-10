
project "ImGui"
    location "./"
    kind "StaticLib"
    language "c++"
    staticruntime "off"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "./**.h",
        "./**.cpp",
    }
    includedirs
    {
        "./",
        "./imgui",
        "./ImGuizmo",
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