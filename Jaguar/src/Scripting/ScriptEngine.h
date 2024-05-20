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
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoClassField MonoClassField;
}


enum class ScriptFieldType
{
	None = 0,
	Float, Double, Vector2, Vector3, Vector4,
	Int, Uint, Bool, Short, Byte, Long, Entity
};


struct ScriptField
{
	ScriptFieldType type;
	std::string name;
	MonoClassField* field;
};

struct ScriptFieldInstance
{
	ScriptField field;

	template<typename T>
	inline T GetValue()
	{
		return *(T*)m_Buffer;
	}

	template<typename T>
	inline void SetFieldValue(T value)
	{
		static_assert(sizeof(T) <= 8);

		// logh(value);
		// logh(*(T*)m_Data);
		memcpy(m_Buffer, &value, sizeof(T));
	}

	const char* GetBuffer() const { return m_Buffer; }

private:
	char m_Buffer[8];
};

class ScriptClass
{
public:
	ScriptClass() = default;
	ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore = false);

	MonoObject* Instantiate();
	MonoMethod* GetMethod(const std::string& name, int peraCount);
	MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);


	inline std::unordered_map<std::string, ScriptField> GetFields() { return m_Fields; };

private:
	std::string m_classNamespace;
	std::string m_className;

	std::unordered_map<std::string, ScriptField> m_Fields;

	MonoClass* m_MonoClass = nullptr;

	friend class ScriptEngine;
};


class JAGUAR_API ScriptInstance
{
public:
	ScriptInstance(Ref<ScriptClass>, Entity entity);


	void InvokeOnCreateMethod();
	void InvokeOnUpdateMethod(float dt);

	inline Ref<ScriptClass> GetScriptClass() {	return m_ScriptCLass; };

	template<typename T>
	inline T GetFieldValue(std::string name)
	{
		GetFieldValueinternal(name, s_FieldValueBuffer);
		return *(T*)s_FieldValueBuffer;
		
	}
	
	template<typename T>
	inline void SetFieldValue(std::string name,const T& value)
	{
		SetFieldValueinternal(name, &value);		
	}

private:
	void GetFieldValueinternal(std::string name, void* buffer);
	void SetFieldValueinternal(std::string name, const void* value);


	Ref<ScriptClass> m_ScriptCLass;

	MonoObject* m_Instance = nullptr;
	MonoMethod* Constructor = nullptr;
	MonoMethod* OnCreateMethod = nullptr;
	MonoMethod* OnUpdateMethod = nullptr;


	inline static char s_FieldValueBuffer[16];
};

class JAGUAR_API ScriptEngine 
{
public:
	static bool EntityClassExists(const std::string& fullClassName);
	static void Init();
	static void ShutDown();
	static void LoadAssembly(const std::filesystem::path filepath);
	static void LoadAppAssembly(const std::filesystem::path filepath);
	static void LoadAssemblyClasses();
	static void OnRuntimeStart(Scene* scene);
	static void OnEditorStart(Scene* scene);
	static void OnRuntimeStop();
	static void OnCreateEntity(Entity entity);
	static void OnUpdateEntity(Entity entity, float deltatime);
	static Scene* GetSceneContext();
	static MonoObject* InstantiateClass(MonoClass* monoClass);
	
	static std::unordered_map<std::string, Ref<ScriptClass>> GetEntityClasses();
	static Ref<ScriptClass> GetEntityClass(std::string name);

	static MonoImage* GetCoreAssemblyImage();

	static Ref<ScriptInstance> GetEntityScriptInstance(Jaguar::UUID uuid);

	static ScriptFieldInstance GetScriptFieldInstance(std::string name, Jaguar::UUID uuid);
	static void SetScriptFieldInstance(std::string name, Jaguar::UUID uuid, ScriptFieldInstance scriptFieldInstance);
	static void debug(Jaguar::UUID id);


private:
	static void InitMono();
	static void ShutDownMono();

	friend class ScriptGlue;
	friend class ScriptClass;

};


