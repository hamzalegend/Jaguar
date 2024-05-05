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
            Console.WriteLine(m_uuid);
        }

        void OnUpdate(float deltatime)
        {
            float speed = 5;
            Vector3 velocity = Vector3.Zero;

            if (Input.GetKeyDown(KeyCode.W))
                velocity.Y += 1;    
            
            if (Input.GetKeyDown(KeyCode.S))
                velocity.Y -= 1;    
            
            if (Input.GetKeyDown(KeyCode.D))
                velocity.X += 1;    
            
            if (Input.GetKeyDown(KeyCode.A))
                velocity.X -= 1;    


            Vector3 translation = Translation;
            translation += velocity * speed * deltatime;
            Translation = translation;

        }


    }
}
