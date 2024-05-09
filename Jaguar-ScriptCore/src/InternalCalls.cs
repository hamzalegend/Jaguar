using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Jaguar
{
    internal static class InternalCalls
    {
        // Entity
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static bool Entity_HasComponent(ulong uuid,Type ComponentType );

        // Transform
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void TransformComponent_GetTranslation(ulong uuid, out Vector3 v);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void TransformComponent_SetTranslation(ulong uuid, ref Vector3 v);

        // SpriteRendererComponent
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void SpriteRendererComponent_GetColor(ulong uuid, out Vector3 v);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void SpriteRendererComponent_SetColor(ulong uuid, ref Vector3 v);
        
        // RigidBody 2D
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void RigidBody2DComponent_ApplyLinarImpulse(ulong uuid, ref Vector2 impulse, ref Vector2 WorldPosition, bool wake);


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void RigidBody2DComponent_ApplyLinarImpulseToCenter(ulong uuid, ref Vector2 impulse, bool wake);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void RigidBody2DComponent_ApplyForceToCenter(ulong uuid, ref Vector2 force, bool wake);

        // Input
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static bool Input_GetKeyDown(KeyCode code);


    }
}
