using System;
using System.Runtime.CompilerServices;

namespace Jaguar
{

    public struct Vector3
    {
        public float X, Y, Z;

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

    }
    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void NativeLog(string text, int par);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void NativeLogv(ref Vector3 v, ref Vector3 outResult);
    }
    

    public class Entity
    {
        public Entity()
        {
            System.Console.WriteLine("Hello World From C#!");
            InternalCalls.NativeLog("Hamza", 8058);
            Vector3 pos = new Vector3(5, 2.5f, 1);
            Vector3 ourout = new Vector3();
            InternalCalls.NativeLogv(ref pos, ref ourout);
            Console.WriteLine(ourout.X + ", " + ourout.Y +", " +ourout.Z);
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

        
    }
    
}