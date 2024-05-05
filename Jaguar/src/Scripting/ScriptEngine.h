#pragma once

#include <filesystem>
#include <string>

#include <core/DEFINES.h>
#include <core/UUID.h>

#include <unordered_map>
#include "Scene/Entity.h"

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
}

class ScriptClass
{
public:
	ScriptClass() = default;
	ScriptClass(const std::string& classNamespace, const std::string& className);

	MonoObject* Instantiate();
	MonoMethod* GetMethod(const std::string& name, int peraCount);
	MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);

private:
	std::string m_classNamespace;
	std::string m_className;

	MonoClass* m_MonoClass = nullptr;
};


class JAGUAR_API ScriptEngine 
{
public:
	static bool EntityClassExists(const std::string& fullClassName);
	static void Init();
	static void ShutDown();
	static void LoadAssembly(const std::filesystem::path filepath);
	static void LoadAssemblyClasses(MonoAssembly* assembly);
	static void OnRuntimeStart(Scene* scene);
	static void OnRuntimeStop();
	static void OnCreateEntity(Entity entity);
	static void OnUpdateEntity(Entity entity, float deltatime);
	static Scene* GetSceneContext();
	static MonoObject* InstantiateClass(MonoClass* monoClass);
	
	static std::unordered_map<std::string, Ref<ScriptClass>> GetEntityClasses();

private:
	static void InitMono();
	static void ShutDownMono();

	friend class ScriptClass;

};


class ScriptInstance
{
public:
	ScriptInstance(Ref<ScriptClass>, Entity entity);


	void InvokeOnCreateMethod();
	void InvokeOnUpdateMethod(float dt);

private:
	Ref<ScriptClass> m_ScriptCLass;

	MonoObject* m_Instance = nullptr;
	MonoMethod* Constructor = nullptr;
	MonoMethod* OnCreateMethod = nullptr;
	MonoMethod* OnUpdateMethod = nullptr;

};

