#pragma once

#include <filesystem>
#include <string>

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
}


class ScriptEngine
{
public:
	static void Init();
	static void ShutDown();
	static void LoadAssembly(const std::filesystem::path filepath);
	static MonoObject* InstantiateClass(MonoClass* monoClass);
	
private:
	static void InitMono();
	static void ShutDownMono();

	friend class ScriptClass;

};

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