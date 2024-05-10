project "Editor"
    location "."
    kind "ConsoleApp"
    language "c++"
    staticruntime "off"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "../%{prj.name}/src/**.h",
        "../%{prj.name}/src/**.cpp",
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