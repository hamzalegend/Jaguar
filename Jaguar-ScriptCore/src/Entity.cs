using System;
using System.Runtime.CompilerServices;

namespace Jaguar
{
    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void NativeLog(string text, int par);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void NativeLogv(ref Vector3 v, ref Vector3 outResult);




        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void Entity_GetTranslation (ulong uuid, out Vector3 v);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void Entity_SetTranslation (ulong uuid, ref Vector3 v);


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static bool Input_GetKeyDown(KeyCode code);


    }


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
                InternalCalls.Entity_GetTranslation(m_uuid, out Vector3 Translation);
                return Translation;
            }
            set
            {
                InternalCalls.Entity_SetTranslation(m_uuid, ref value);
            }
        }



        public void PrintMessage()
        {
            System.Console.WriteLine("this is a Message!");
        }
        public void PrintMessageWithString(string msg, int t)
        {
            for (int i = 0; i < (int)t / 2; i++)
                System.Console.WriteLine("C# Says: " + msg);
        }

        public ulong m_uuid;
        
    }
    
}