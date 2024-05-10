project "Jaguar-ScriptCore"
    location "./"
    kind "SharedLib"
    language "C#"
    dotnetframework "4.7.2"
    -- staticruntime "off"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "./src/**.cs",
        "./src/**.cs",
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