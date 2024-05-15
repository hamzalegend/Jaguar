project "Jaguar-ScriptCore"
    location "./"
    kind "SharedLib"
    language "C#"
    dotnetframework "4.7.2"
    -- staticruntime "off"

    targetdir ("../Editor/res/Scripts")
    objdir ("../Editor/res/Scripts/int")

    files
    {
        "./src/**.cs",
        "./src/**.cs",
    }

    -- postbuildcommands {
    --     "{COPY} " .. JaguarRootDir .. "bin/" .. outputdir .. "/%{prj.name}/Jaguar-ScriptCore.dll %{wks.location}/Editor/res/Scripts",
    --     "{COPY} " .. JaguarRootDir .. "bin/" .. outputdir .. "/%{prj.name}/Jaguar-ScriptCore.dll %{wks.location}/SandBox/res/Scripts",
    -- }

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