#include "Core/Application.h"
#include "Editor.h"

int main()
{
	Application app;
	Ref<EditorLayer> Editor= MakeRef<EditorLayer>("C:/Users/ASUS/Desktop/Projects/Demo");
	app.PushLayer(Editor);
	app.Run();
	return 0;
}