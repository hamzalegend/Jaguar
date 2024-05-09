using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

using Jaguar;

namespace Sandbox
{
    public class Player : Entity
    {
        private TransformComponent transformComponent;
        private RigidBody2DComponent rb;

        void OnCreate()
        {
            Console.WriteLine("OnCreate();");
            Console.WriteLine(m_uuid);

            Random rnd = new Random();
            GetComponent<SpriteRendererComponent>().Color = new Vector3((float)rnd.NextDouble(), (float)rnd.NextDouble(), (float)rnd.NextDouble());

            transformComponent = GetComponent<TransformComponent>();
            rb = GetComponent<RigidBody2DComponent>();
        }
        void OnUpdate(float deltatime)
        {
            float speed = 1000000f;
            Vector3 velocity = Vector3.Zero;
             
            if (Input.GetKeyDown(KeyCode.W))
                velocity.Y += 1;

            if (Input.GetKeyDown(KeyCode.S))
                velocity.Y -= 1;

            if (Input.GetKeyDown(KeyCode.D))
                velocity.X += 1;

            if (Input.GetKeyDown(KeyCode.A))
                velocity.X -= 1;



            rb.RigidBody2DComponent_ApplyForceToCenter(velocity.XY, true);
            // Vector3 translation = transformComponent.Translation;
            // translation += velocity * speed * deltatime;
            // transformComponent.Translation = translation;




        }
    }
}
