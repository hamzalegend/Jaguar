using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Jaguar
{
    public class Component 
    {
        public Entity Entity { get; internal set; }
    }

    public class TransformComponent : Component
    {
        public Vector3 Translation 
        { 
            get 
            {
                InternalCalls.TransformComponent_GetTranslation(Entity.m_uuid, out Vector3 translation);
                return translation;
            }
            
            set
            {
                InternalCalls.TransformComponent_SetTranslation(Entity.m_uuid, ref value);
            }
        }
    }
    
    public class RigidBody2DComponent : Component
    {
        public void ApplyLinarImpulse(Vector2 impulse, Vector2 WorldPosition, bool wake)
        {
            InternalCalls.RigidBody2DComponent_ApplyLinarImpulse(Entity.m_uuid,ref impulse,ref WorldPosition, wake);
        }
        
        public void ApplyLinarImpulseToCenter(Vector2 impulse, bool wake)
        {
            InternalCalls.RigidBody2DComponent_ApplyLinarImpulseToCenter(Entity.m_uuid,ref impulse, wake);
        }
        public void ApplyForceToCenter(Vector2 force, bool wake)
        {
            InternalCalls.RigidBody2DComponent_ApplyForceToCenter(Entity.m_uuid, ref force, wake);
        }
    }

    public class SpriteRendererComponent : Component
    {
        public Vector3 Color
        {
            get
            {
                InternalCalls.SpriteRendererComponent_GetColor(Entity.m_uuid, out Vector3 color);
                return color;
            }
            set
            {
                InternalCalls.SpriteRendererComponent_SetColor(Entity.m_uuid, ref value);
            }
        }
    }
}
