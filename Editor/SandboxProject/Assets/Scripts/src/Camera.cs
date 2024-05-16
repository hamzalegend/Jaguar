﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

using Jaguar;

namespace Sandbox
{
    public class Camera : Entity
    {
        public float speed = 5;
        public float time;
        void OnUpdate(float deltatime)
        {
            time += deltatime;
            
            Vector3 velocity = Vector3.Zero;

            if (Input.GetKeyDown(KeyCode.Up))
                velocity.Y += 1;

            if (Input.GetKeyDown(KeyCode.Down))
                velocity.Y -= 1;

            if (Input.GetKeyDown(KeyCode.Left))
                velocity.X -= 1;

            if (Input.GetKeyDown(KeyCode.Right))
                velocity.X += 1;
            Vector3 translation = Translation;
            translation.X = 2*(float)Math.Sin(time);
            translation += velocity * speed * deltatime;
            Translation = translation;

        }
    }
}
