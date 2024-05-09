#include "ScriptGlue.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/reflection.h"

#include <string>
#include <fstream>
#include <iostream>

#include <math/math.h>
#include <Scene/Scene.h>
#include "ScriptEngine.h"

#include "Core/KeyCodes.h"
#include <window/Input.h>

#include "Box2D/b2_body.h"

static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_HasComponentFuncs;

#define JR_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Jaguar.InternalCalls::"#Name, Name)

static bool Entity_HasComponent(Jaguar::UUID uuid, MonoReflectionType* type)
{
    MonoType* managedtype = mono_reflection_type_get_type(type);

    Entity e = ScriptEngine::GetSceneContext()->GetEntityByUUID(uuid);
    return s_HasComponentFuncs.at(managedtype)(e);
}

//
static void TransformComponent_GetTranslation(Jaguar::UUID uuid, glm::vec3* outv)
{
    Scene* scene = ScriptEngine::GetSceneContext();
    *outv = scene->GetEntityByUUID(uuid).GetComponent<TransformComponent>().Position;
}

static void TransformComponent_SetTranslation(Jaguar::UUID uuid, glm::vec3* v)
{
    Scene* scene = ScriptEngine::GetSceneContext();
    scene->GetEntityByUUID(uuid).GetComponent<TransformComponent>().Position = *v;
    
}

// SpriteRendererComponent
static void SpriteRendererComponent_GetColor(Jaguar::UUID uuid, glm::vec3* outv)
{
    Scene* scene = ScriptEngine::GetSceneContext();
    *outv = scene->GetEntityByUUID(uuid).GetComponent<SpriteRendererComponent>().sprite.color;
}

static void SpriteRendererComponent_SetColor(Jaguar::UUID uuid, glm::vec3* v)
{
    Scene* scene = ScriptEngine::GetSceneContext();
    scene->GetEntityByUUID(uuid).GetComponent<SpriteRendererComponent>().sprite.color = glm::vec4(*v, 1.0);
    
}


// RigidBody2DComponent
static void RigidBody2DComponent_ApplyLinarImpulse(Jaguar::UUID uuid, glm::vec2* impulse, glm::vec2* point, bool wake)
{
    Scene* scene = ScriptEngine::GetSceneContext();
    
    Entity entity = scene->GetEntityByUUID(uuid);
    

    auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
    b2Body* body = (b2Body*)rb2d.RunTimeBody;
    body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(point->x, point->y), wake);
}

static void RigidBody2DComponent_ApplyLinarImpulseToCenter(Jaguar::UUID uuid, glm::vec2* impulse, bool wake)
{
    Scene* scene = ScriptEngine::GetSceneContext();

    Entity entity = scene->GetEntityByUUID(uuid);


    auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
    b2Body* body = (b2Body*)rb2d.RunTimeBody;
    body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
}

static void RigidBody2DComponent_ApplyForceToCenter(Jaguar::UUID uuid, glm::vec2* impulse, bool wake)
{
    Scene* scene = ScriptEngine::GetSceneContext();

    Entity entity = scene->GetEntityByUUID(uuid);


    auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
    b2Body* body = (b2Body*)rb2d.RunTimeBody;
    body->ApplyForceToCenter(b2Vec2(impulse->x, impulse->y), wake);
}


//
static bool Input_GetKeyDown(Jaguar::KeyCode keycode)
{
    return Input::GetKey(keycode);
}


void ScriptGLue::RegisterFunctions()
{
    JR_ADD_INTERNAL_CALL(Entity_HasComponent);

    JR_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
    JR_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);

    JR_ADD_INTERNAL_CALL(SpriteRendererComponent_GetColor);
    JR_ADD_INTERNAL_CALL(SpriteRendererComponent_SetColor);

    JR_ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyLinarImpulse);
    JR_ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyLinarImpulseToCenter);
    JR_ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyForceToCenter);

    JR_ADD_INTERNAL_CALL(Input_GetKeyDown);
}

template <typename... T>
void RegisterComponent()
{
    ([]() {

        std::string typeName = typeid(T).name();
        size_t pos = typeName.find_last_of(" ");
        std::string structname = typeName.substr(pos + 1);
        std::string ManagedTypeName = std::string("Jaguar.") + structname;
        // std::cout << ManagedTypeName.c_str() << "\n";

        MonoType* managedType = mono_reflection_type_from_name(ManagedTypeName.data(), ScriptEngine::GetCoreAssemblyImage());

        if (!managedType)
        {
            std::cout << "[ERROR] Could not find type component '" << ManagedTypeName.c_str() << "'\n";
        }
        s_HasComponentFuncs[managedType] = [](Entity e) { return e.HasComponent<T>(); };
        }(), ...);
}

template <typename... T>
void RegisterComponent(ComponentGroup<T ... >)
{
    RegisterComponent<T ...>();
}

void ScriptGLue::RegisterComponents()
{
    RegisterComponent(AllComponents{});
    // for (typename T : ComponentGroup)
    // {
    // 
    // }
}
