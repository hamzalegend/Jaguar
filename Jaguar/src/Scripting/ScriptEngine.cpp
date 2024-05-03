#include "ScriptEngine.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

#include <string>
#include <fstream>
#include <iostream>

#include <math/math.h>

#include "ScriptGlue.h"
#include <Core/DEFINES.h>

#include <unordered_map>

namespace Utils
{

    static char *ReadBytes(const std::string &filepath, uint32_t *outSize)
    {
        std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

        if (!stream)
        {
            // Failed to open the file
            return nullptr;
        }

        std::streampos end = stream.tellg();
        stream.seekg(0, std::ios::beg);
        uint32_t size = end - stream.tellg();

        if (size == 0)
        {
            // File is empty
            return nullptr;
        }

        char *buffer = new char[size];
        stream.read((char *)buffer, size);
        stream.close();

        *outSize = size;
        return buffer;
    }

    static MonoAssembly *LoadMonoAssembly(const std::filesystem::path &assemblyPath)
    {
        uint32_t fileSize = 0;
        char *fileData = ReadBytes(assemblyPath.string(), &fileSize);

        // NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
        MonoImageOpenStatus status;
        MonoImage *image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

        if (status != MONO_IMAGE_OK)
        {
            const char *errorMessage = mono_image_strerror(status);
            std::cout << errorMessage << std::endl;
            return nullptr;
        }

        MonoAssembly *assembly = mono_assembly_load_from_full(image, assemblyPath.string().c_str(), &status, 0);
        mono_image_close(image);

        // Don't forget to free the file data
        delete[] fileData;

        return assembly;
    }

    static void PrintAssemblyTypes(MonoAssembly *assembly)
    {
        MonoImage *image = mono_assembly_get_image(assembly);
        const MonoTableInfo *typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
        int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

        for (int32_t i = 0; i < numTypes; i++)
        {
            uint32_t cols[MONO_TYPEDEF_SIZE];
            mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

            const char *nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
            const char *name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

            printf("%s.%s\n", nameSpace, name);
        }
    }
}

struct ScriptEngineData
{
    MonoDomain *RootDomain = nullptr;
    MonoDomain *AppDomain = nullptr;

    MonoAssembly *CoreAssembly = nullptr;
    MonoImage *CoreAssemblyImage = nullptr;

    ScriptClass EntityClass;

    std::unordered_map<std::string, Ref<ScriptClass>> EntityClasses;
};

static ScriptEngineData *s_Data;

void ScriptEngine::Init()
{
    s_Data = new ScriptEngineData;

    InitMono();
    LoadAssembly("res/Scripts/Jaguar-ScriptCore.dll");

    LoadAssemblyClasses(s_Data->CoreAssembly);

    ScriptGLue::RegisterFunctions();

   //  MonoObject* instance = s_Data->EntityClass.Instantiate();
   // 
   //  MonoMethod* OnCreateFunc = s_Data->EntityClass.GetMethod("OnCreate", 0);
   //  s_Data->EntityClass.InvokeMethod(instance, OnCreateFunc, nullptr);

    if (0) {
            // retrieve and instantiate claass with constructor
            s_Data->EntityClass = ScriptClass("Jaguar", "Entity");
        MonoObject* instance = s_Data->EntityClass.Instantiate();

        // call method
        MonoMethod* printMessageFunc = s_Data->EntityClass.GetMethod("PrintMessage", 0);
        s_Data->EntityClass.InvokeMethod(instance, printMessageFunc, nullptr);

        MonoMethod* printMessageWithParamsFunc = s_Data->EntityClass.GetMethod("PrintMessageWithString", 2);

        int myint = 10;
        MonoString* mystr = mono_string_new(s_Data->AppDomain, "hello 5 times!");
        void* perams[]{
            mystr,
            &myint,
        };
        s_Data->EntityClass.InvokeMethod(instance, printMessageWithParamsFunc, perams);
    }
}

void ScriptEngine::ShutDown()
{
    ShutDownMono();
    delete s_Data;
}

void ScriptEngine::LoadAssembly(const std::filesystem::path filepath)
{

    s_Data->AppDomain = mono_domain_create_appdomain((char *)"JaguarScriptRuntime", nullptr);
    mono_domain_set(s_Data->AppDomain, true);

    // move this
    s_Data->CoreAssembly = Utils::LoadMonoAssembly(filepath.string());
    s_Data->CoreAssemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);
    Utils::PrintAssemblyTypes(s_Data->CoreAssembly);    
}

void ScriptEngine::LoadAssemblyClasses(MonoAssembly *assembly)
{
    s_Data->EntityClasses.clear();

    MonoImage *image = mono_assembly_get_image(assembly);
    const MonoTableInfo *typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
    int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

    MonoClass *EntityClass = mono_class_from_name(s_Data->CoreAssemblyImage, "Jaguar", "Entity");
    for (int32_t i = 0; i < numTypes; i++)
    {
        uint32_t cols[MONO_TYPEDEF_SIZE];
        mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

        const char *nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
        const char *name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

        MonoClass *monoClass = mono_class_from_name(s_Data->CoreAssemblyImage, nameSpace, name);
        bool isEntity = mono_class_is_subclass_of(monoClass, EntityClass, false);

        std::string fullname;
        if (strlen(name) != 0)
            fullname = std::string(nameSpace) + "." + std::string(name);
        else
            fullname = name;

        if (monoClass == EntityClass) continue;
        if (isEntity)
            s_Data->EntityClasses[fullname] = MakeRef<ScriptClass>(nameSpace, name);
    }
}


std::unordered_map<std::string, Ref<ScriptClass>> GetEntityClasses()
{
    return s_Data->EntityClasses;
}

bool ScriptEngine::EntityClassExists(const std::string& fullClassName)
{
    return s_Data->EntityClasses.find(fullClassName) != s_Data->EntityClasses.end();
}

void ScriptEngine::InitMono()
{
    mono_set_assemblies_path("mono/lib");

    MonoDomain *rootDomain = mono_jit_init("JaguarJITRuntime");
    s_Data->RootDomain = rootDomain;
}

void ScriptEngine::ShutDownMono()
{
    // mono_domain_unload(s_Data->AppDomain);
    s_Data->AppDomain = nullptr;

    // mono_jit_cleanup(s_Data->RootDomain);
    s_Data->RootDomain = nullptr;
}

MonoObject *ScriptEngine::InstantiateClass(MonoClass *monoClass)
{
    MonoObject *instance = mono_object_new(s_Data->AppDomain, monoClass);
    mono_runtime_object_init(instance);
    return instance;
}

//

ScriptClass::ScriptClass(const std::string &classNamespace, const std::string &className)
    : m_className(className), m_classNamespace(classNamespace)
{
    m_MonoClass = mono_class_from_name(s_Data->CoreAssemblyImage, m_classNamespace.c_str(), m_className.c_str());
}

MonoObject *ScriptClass::Instantiate()
{
    return ScriptEngine::InstantiateClass(m_MonoClass);
}

MonoMethod *ScriptClass::GetMethod(const std::string &name, int paraCount)
{
    return mono_class_get_method_from_name(m_MonoClass, name.c_str(), paraCount);
}

MonoObject *ScriptClass::InvokeMethod(MonoObject *instance, MonoMethod *method, void **params)
{
    return mono_runtime_invoke(method, instance, params, nullptr);
}


//


ScriptInstance::ScriptInstance(Ref<ScriptClass> scriptClass)
    :m_ScriptCLass(scriptClass)
{
    m_Instance = m_ScriptCLass->Instantiate();
    OnCreateMethod = m_ScriptCLass->GetMethod("OnCreate", 0);
    OnUpdateMethod = m_ScriptCLass->GetMethod("OnUpdate", 1);
}

void ScriptInstance::InvokeOnCreateMethod() 
{
    m_ScriptCLass->InvokeMethod(m_Instance, OnCreateMethod, nullptr);
}

void ScriptInstance::InvokeOnUpdateMethod(float dt)
{
    void* param = &dt;
    m_ScriptCLass->InvokeMethod(m_Instance, OnUpdateMethod, &param);

}
