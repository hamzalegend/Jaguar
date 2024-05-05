using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Jaguar
{
    public class Input
    {
        static public bool GetKeyDown(KeyCode keycode)
        {
            return InternalCalls.Input_GetKeyDown(keycode);
        }
    }
}
