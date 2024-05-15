using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;

using Jaguar;

namespace Sandbox
{
    public class Player : Entity
    {
        private TransformComponent transformComponent;
        private RigidBody2DComponent rb;

        private bool prev;

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
            float speed = 500;
            Vector3 velocity = Vector3.Zero;
             
            if (Input.GetKeyDown(KeyCode.W))
                velocity.Y += speed;

            if (Input.GetKeyDown(KeyCode.S))
                velocity.Y -= speed;

            if (Input.GetKeyDown(KeyCode.D))
                velocity.X += speed;

            if (Input.GetKeyDown(KeyCode.A))
                velocity.X -= speed;

            if (Input.GetKeyDown(KeyCode.Space) && !prev)
            {
                rb.ApplyLinarImpulseToCenter(new Vector2(0, 1f), true);
            }
            prev = Input.GetKeyDown(KeyCode.Space);
  
            // Console.WriteLine((velocity.XY * deltatime));
            rb.ApplyForceToCenter(velocity.XY * deltatime, true);
        }
    }
}
