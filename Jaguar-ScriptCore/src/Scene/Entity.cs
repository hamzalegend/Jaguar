using System;

namespace Jaguar
{
    public class Entity
    {
        protected Entity() { m_uuid = 0; }

        internal Entity(ulong uuid)
        {
            m_uuid = uuid;
        }

        public Vector3 Translation
        {
            get
            {
                InternalCalls.TransformComponent_GetTranslation(m_uuid, out Vector3 Translation);
                return Translation;
            }
            set
            {
                InternalCalls.TransformComponent_SetTranslation(m_uuid, ref value);
            }
        }
        public bool HasComponent<T>() where T : Component, new()
        {
            Type type = typeof(T);
            return InternalCalls.Entity_HasComponent(m_uuid, type);
        }
        public T GetComponent<T>() where T : Component, new()
        {
            if (!HasComponent<T>())
            {
                Console.WriteLine("[ERROR]doesnt have Component!");
                return null;
            }

            T component = new T() { Entity = this };
            return component;
        }

        public ulong m_uuid;

    }

}