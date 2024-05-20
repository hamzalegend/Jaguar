#include "ScriptEngine.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/tabledefs.h"

#include <string>
#include <fstream>
#include <iostream>

#include <math/math.h>

#include "ScriptGlue.h"
#include <Core/DEFINES.h>
#include <Core/UUID.h>

#include <unordered_map>
#include <Engine.h>

#include "scene/Components.h"
#include "Core/UUID.h"

static std::unordered_map<std::string, ScriptFieldType> s_ScriptFieldTypeMap =
{
    {"System.Single", ScriptFieldType::Float},
    {"System.Double", ScriptFieldType::Double},
    {"System.Boolean", ScriptFieldType::Bool},
    {"System.Int64", ScriptFieldType::Int},
    {"System.UInt32", ScriptFieldType::Uint},
    {"System.Int16", ScriptFieldType::Short},
    {"System.Byte.", ScriptFieldType::Byte},
    {"System.Long", ScriptFieldType::Long},

    {"Jaguar.Vector2", ScriptFieldType::Vector2},
    {"Jaguar.Vector3", ScriptFieldType::Vector3},
    {"Jaguar.Vector4", ScriptFieldType::Vector4},
    {"Jaguar.Entity", ScriptFieldType::Entity},
};

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



    ScriptFieldType MonoTypeToScriptFieldType(MonoType* monotype)
    {
        std::string name = mono_type_get_name(monotype);
        if (s_ScriptFieldTypeMap.find(name) == s_ScriptFieldTypeMap.end()) return ScriptFieldType::None;
        return s_ScriptFieldTypeMap.at(name);
    }

    const char* ScriptFieldTypeToString(ScriptFieldType type)
    {
        switch (type)
        {
        case ScriptFieldType::None:
            return "None";
        case ScriptFieldType::Float:
            return "Float";
        case ScriptFieldType::Double:
            return "Double";
        case ScriptFieldType::Vector2:
            return "Vector2";
        case ScriptFieldType::Vector3:
            return "Vector3";
        case ScriptFieldType::Vector4:
            return "Vector4";
        case ScriptFieldType::Int:
            return "Int";
        case ScriptFieldType::Uint:
            return "Uint";
        case ScriptFieldType::Bool:
            return "Bool";
        case ScriptFieldType::Short:
            return "Short";
        case ScriptFieldType::Byte:
            return "Byte";
        case ScriptFieldType::Entity:
            return "Entity";
        }

        return "UnkNONE type";
    }
}

struct ScriptEngineData
{
    MonoDomain *RootDomain = nullptr;
    MonoDomain *AppDomain = nullptr;

    MonoAssembly *CoreAssembly = nullptr;
    MonoImage *CoreAssemblyImage = nullptr;

    MonoAssembly* AppAssembly = nullptr;
    MonoImage* AppAssemblyImage = nullptr;

    ScriptClass EntityClass;

    std::unordered_map<std::string, Ref<ScriptClass>> EntityClasses;
    std::unordered_map<Jaguar::UUID, Ref<ScriptInstance>> EntityInstances;

    std::unordered_map<Jaguar::UUID, std::unordered_map<std::string, ScriptFieldInstance>> ScriptFieldInstances;

    Scene* SceneContext;
};

static ScriptEngineData *s_Data;

void ScriptEngine::Init()
{
    s_Data = new ScriptEngineData;

    InitMono();
    LoadAssembly("res/Scripts/Jaguar-ScriptCore.dll");
    LoadAppAssembly("SandboxProject/Assets/Scripts/Binaries/Sandbox.dll");

    LoadAssemblyClasses();

    ScriptGLue::RegisterComponents();
    ScriptGLue::RegisterFunctions();

   //  MonoObject* instance = s_Data->EntityClass.Instantiate();
   // 
   //  MonoMethod* OnCreateFunc = s_Data->EntityClass.GetMethod("OnCreate", 0);
   //  s_Data->EntityClass.InvokeMethod(instance, OnCreateFunc, nullptr);

    s_Data->EntityClass = ScriptClass("Jaguar", "Entity", true);
    if (0) {
            // retrieve and instantiate claass with constructor
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
    // Utils::PrintAssemblyTypes(s_Data->CoreAssembly);    
}
void ScriptEngine::LoadAppAssembly(const std::filesystem::path filepath)
{
    // move this
    s_Data->AppAssembly = Utils::LoadMonoAssembly(filepath.string());
    s_Data->AppAssemblyImage = mono_assembly_get_image(s_Data->AppAssembly);
    // Utils::PrintAssemblyTypes(s_Data->AppAssembly);
}

std::unordered_map<std::string, Ref<ScriptClass>> ScriptEngine::GetEntityClasses()
{
    return s_Data->EntityClasses;
}

Ref<ScriptClass> ScriptEngine::GetEntityClass(std::string name)
{
    auto& it = s_Data->EntityClasses.find(name);
    if (it != s_Data->EntityClasses.end())
        return it->second;
    return nullptr;
}

void ScriptEngine::LoadAssemblyClasses()
{
    s_Data->EntityClasses.clear();

    const MonoTableInfo *typeDefinitionsTable = mono_image_get_table_info(s_Data->AppAssemblyImage, MONO_TABLE_TYPEDEF);
    int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

    MonoClass* EntityClass = mono_class_from_name(s_Data->CoreAssemblyImage, "Jaguar", "Entity");
    for (int32_t i = 0; i < numTypes; i++)
    {
        uint32_t cols[MONO_TYPEDEF_SIZE];
        mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

        const char *nameSpace = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
        const char *name = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAME]);

        MonoClass *monoClass = mono_class_from_name(s_Data->AppAssemblyImage, nameSpace, name);
        bool isEntity = mono_class_is_subclass_of(monoClass, EntityClass, false);

        std::string fullname;
        if (strlen(name) != 0)
            fullname = std::string(nameSpace) + "." + std::string(name);
        else
            fullname = name;

        if (monoClass == EntityClass) continue;

        if (!isEntity) continue;
        Ref<ScriptClass> SC = MakeRef<ScriptClass>(nameSpace, name);
        s_Data->EntityClasses[fullname] = SC;
        
        void* fields = nullptr;
        MonoClassField* field;
        while (MonoClassField* field = mono_class_get_fields(monoClass, &fields))
        {
            uint32_t flags = mono_field_get_flags(field);
            if (flags & FIELD_ATTRIBUTE_PUBLIC)
            {
                // std::cout << mono_field_get_name(field) << " is public" << ";\n";

                MonoType* type = mono_field_get_type(field);
                ScriptFieldType Fieldtype = Utils::MonoTypeToScriptFieldType(type);
                std::string fieldname = mono_field_get_name(field);
                SC->m_Fields[fieldname] = { Fieldtype, fieldname, field};
            }
        }   
    }

}

void ScriptEngine::OnRuntimeStart(Scene* scene)
{
    s_Data->SceneContext = scene;
}

void ScriptEngine::OnEditorStart(Scene* scene)
{
    for (auto& e : scene->m_Registry.view<ScriptComponent>())
    {
        Entity entity = { e, scene };
        const auto& SC = entity.GetComponent<ScriptComponent>();
        Jaguar::UUID uuid = entity.GetComponent<UUIDComponent>().uuid;



        
        if (!ScriptEngine::EntityClassExists(SC.ClassName))  return;

        auto& scriptClass = ScriptEngine::GetEntityClass(SC.ClassName);
        auto& fields = scriptClass->GetFields();

        std::unordered_map<std::string, ScriptField>::iterator it;
        for (it = fields.begin(); it != fields.end(); it++)
        {
            ScriptFieldInstance instance;
            instance.field = it->second;
            instance.SetFieldValue(
                ScriptInstance(scriptClass, entity).GetFieldValue<float>(it->first) // if not saved on disk
            );
            
            s_Data->ScriptFieldInstances[uuid][it->first] = instance;
        }
    }
}


void ScriptEngine::OnRuntimeStop()
{
    // s_Data->EntityClasses.clear();
}


void ScriptEngine::OnCreateEntity(Entity entity)
{
    auto sc = entity.GetComponent<ScriptComponent>();
    if (ScriptEngine::EntityClassExists(sc.ClassName))
    {
        Ref<ScriptInstance> instance = MakeRef<ScriptInstance>(s_Data->EntityClasses[sc.ClassName], entity);
        Jaguar::UUID uuid = entity.GetComponent<UUIDComponent>().uuid;

        s_Data->EntityInstances[uuid] = instance;

        // fields stuff
        auto& mp = s_Data->ScriptFieldInstances.find(uuid);
        std::unordered_map<std::string, ScriptFieldInstance>::iterator it;
        for (it = mp->second.begin(); it != mp->second.end(); it++)
        {
            auto StringFieldMap = s_Data->ScriptFieldInstances.find(uuid)->second;
            auto value = StringFieldMap.find(it->first)->second.GetValue<float>();
            instance->SetFieldValue(it->first, value);
            it->second.SetFieldValue(value);
        }

        instance->InvokeOnCreateMethod();
    }
}

void ScriptEngine::OnUpdateEntity(Entity entity, float deltatime)
{
    auto sc = entity.GetComponent<ScriptComponent>();
    
    // Ref<ScriptInstance> instance = MakeRef<ScriptInstance>(s_Data->EntityClasses[sc.ClassName], entity);
    Ref<ScriptInstance> instance = s_Data->EntityInstances[entity.GetComponent<UUIDComponent>().uuid];
    s_Data->EntityInstances[entity.GetComponent<UUIDComponent>().uuid] = instance;
    instance->InvokeOnUpdateMethod(deltatime);
   
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

MonoObject* ScriptEngine::InstantiateClass(MonoClass *monoClass)
{
    MonoObject *instance = mono_object_new(s_Data->AppDomain, monoClass);
    mono_runtime_object_init(instance);
    return instance;
}

MonoImage* ScriptEngine::GetCoreAssemblyImage()
{
    return s_Data->CoreAssemblyImage;
}

Ref<ScriptInstance> ScriptEngine::GetEntityScriptInstance(Jaguar::UUID uuid)
{
    auto it = s_Data->EntityInstances.find(uuid);
    if (it == s_Data->EntityInstances.end()) return nullptr;
    return it->second;
}

ScriptFieldInstance ScriptEngine::GetScriptFieldInstance(std::string name, Jaguar::UUID uuid)
{
    return s_Data->ScriptFieldInstances[uuid][name];
}

void ScriptEngine::SetScriptFieldInstance(std::string name, Jaguar::UUID uuid, ScriptFieldInstance scriptFieldInstance)
{
    s_Data->ScriptFieldInstances[uuid][name] = scriptFieldInstance;
}

static void debug(Jaguar::UUID id)
{
}


//

ScriptClass::ScriptClass(const std::string &classNamespace, const std::string &className, bool isCore)
    : m_className(className), m_classNamespace(classNamespace)
{
    m_MonoClass = mono_class_from_name(isCore ? s_Data->CoreAssemblyImage : s_Data->AppAssemblyImage, m_classNamespace.c_str(), m_className.c_str());
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
    // if crash here then check intenal calls
}

Scene* ScriptEngine::GetSceneContext()
{
    return s_Data->SceneContext;
}

//


ScriptInstance::ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity)
    :m_ScriptCLass(scriptClass)
{
    std::cout << entity.GetComponent<TagComponent>().name << "\n";
    m_Instance = m_ScriptCLass->Instantiate();
    Constructor = s_Data->EntityClass.GetMethod(".ctor", 1); // to get constructor
    OnCreateMethod = m_ScriptCLass->GetMethod("OnCreate", 0);
    OnUpdateMethod = m_ScriptCLass->GetMethod("OnUpdate", 1);

    { // call constructor
        Jaguar::UUID uuid = entity.GetComponent<UUIDComponent>().uuid;
        void* p = &uuid;
        m_ScriptCLass->InvokeMethod(m_Instance, Constructor, &p);
    }

}

void ScriptInstance::InvokeOnCreateMethod() 
{
    if (OnCreateMethod)
        m_ScriptCLass->InvokeMethod(m_Instance, OnCreateMethod, nullptr);
}

void ScriptInstance::InvokeOnUpdateMethod(float dt)
{
    void* param = &dt;
    if (OnUpdateMethod)
        m_ScriptCLass->InvokeMethod(m_Instance, OnUpdateMethod, &param);

}

void ScriptInstance::GetFieldValueinternal(std::string name, void* buffer)
{
    mono_field_get_value(m_Instance, m_ScriptCLass->GetFields().find(name)->second.field, buffer);
}

void ScriptInstance::SetFieldValueinternal(std::string name, const void* value)
{

    mono_field_set_value(m_Instance, m_ScriptCLass->GetFields().find(name)->second.field, (void*)value);
}
