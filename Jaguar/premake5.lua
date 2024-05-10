project "Jaguar"
    location "./"
    kind "SharedLib"
    language "c++"
    staticruntime "off"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp",
    }
    includedirs
    {
        "%{wks.location}/glad/include/",
        "%{wks.location}/Vendor/glfw/include",
        "%{wks.location}/Vendor/glm/include",
        "vendor",
        "src",
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
            "{COPY} %{wks.location}/Vendor/glfw/lib/glfw3.dll %{wks.location}/bin/Debug-windows-x86_64/SandBox",
            "{COPY} %{wks.location}/Vendor/glfw/lib/glfw3.dll %{wks.location}/bin/Debug-windows-x86_64/Editor",
            "{COPY} %{wks.location}/Vendor/assimp/lib/assimp-vc143-mt.dll %{wks.location}/bin/Debug-windows-x86_64/SandBox",
            "{COPY} %{wks.location}/Vendor/assimp/lib/assimp-vc143-mt.dll %{wks.location}/bin/Debug-windows-x86_64/Editor",
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