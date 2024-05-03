using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Jaguar;

namespace Sandbox
{
    public class Player : Entity
    {

        void OnCreate()
        {
            Console.WriteLine("OnCreate();");
        }

        void OnUpdate(float deltatime)
        {
            Console.WriteLine("OnUpdate();");
            Console.WriteLine(deltatime);
        }


    }
}
