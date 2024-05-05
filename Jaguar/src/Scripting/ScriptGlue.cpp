#include "ScriptGlue.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

#include <string>
#include <fstream>
#include <iostream>

#include <math/math.h>
#include <Scene/Scene.h>
#include "ScriptEngine.h"

#include "Core/KeyCodes.h"
#include <window/Input.h>


#define JR_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Jaguar.InternalCalls::"#Name, Name)

static void NativeLog(MonoString* text, int par)
{
    char* cStr = mono_string_to_utf8(text);
    std::cout << cStr << ", " << par << "\n";
    mono_free(cStr);
}

static void NativeLogv(glm::vec3* v, glm::vec3* outResult)
{
    std::cout << v->x << ", " << v->y << ", " << v->z << "\n";
    *outResult = glm::cross(*v, glm::vec3(v->x, v->y, -v->z));
}

static void Entity_GetTranslation(Jaguar::UUID uuid, glm::vec3* outv)
{
    Scene* scene = ScriptEngine::GetSceneContext();
    *outv = scene->GetEntityByUUID(uuid).GetComponent<TransformComponent>().Position;
}

static void Entity_SetTranslation(Jaguar::UUID uuid, glm::vec3* v)
{
    Scene* scene = ScriptEngine::GetSceneContext();
    scene->GetEntityByUUID(uuid).GetComponent<TransformComponent>().Position = *v;
    
}

static bool Input_GetKeyDown(Jaguar::KeyCode keycode)
{
    return Input::GetKey(keycode);
}


void ScriptGLue::RegisterFunctions()
{
	JR_ADD_INTERNAL_CALL(NativeLog);
    JR_ADD_INTERNAL_CALL(NativeLogv);

    JR_ADD_INTERNAL_CALL(Entity_GetTranslation);
    JR_ADD_INTERNAL_CALL(Entity_SetTranslation);

    JR_ADD_INTERNAL_CALL(Input_GetKeyDown);
}
