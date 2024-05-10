
project "glad"
    location "./"
    kind "StaticLib"
    language "c++"
    staticruntime "off"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "./**.h",
        "./**.c",
    }
    includedirs
    {
        "./include/",
        "./src/",
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
