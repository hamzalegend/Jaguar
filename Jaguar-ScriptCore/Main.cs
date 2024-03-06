using System;

namespace Jaguar_ScriptCore
{
    public class Main
    {
        public Main() 
        {
            System.Console.WriteLine("Hello World From C#!");
        }
        public void PrintMessage()
        {
            System.Console.WriteLine("this is a Message!");
        }
        public void PrintMessageWithString(string msg, int t)
        {
            for (int i = 0; i < (int)t/2;i++)
                System.Console.WriteLine("C# Says: " + msg);
        }
    }
}