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
JaguarRootDir = "./"
group "Core"

include "Jaguar"
include "Jaguar-ScriptCore"
group ""
include "SandBox"

group "Tools"
include "Editor"
group ""

    
group "Dependencies"
include "Vendor"    
include "glad"    
include "ImGui"

group ""