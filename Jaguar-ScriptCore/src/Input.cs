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
