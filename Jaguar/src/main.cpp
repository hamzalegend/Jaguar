
#define _CRTDBG_MAP_ALLOC

#include "Core/Application.h"
// #include "client/SandboxLayer/SandBoxLayer.h"
#include "Core/DEFINES.h"
#include <memory>
int main()
{
	Application app;
	// Ref<SandBoxLayer> Layer = MakeRef<SandBoxLayer>();
	// Ref<EditorLayer> Editor= MakeRef<EditorLayer>();
	// app.PushLayer(Layer);
	// app.PushLayer(Editor);
	app.Run();
	return 0;

	// #include <crtdbg.h>
	// _CrtDumpMemoryLeaks(); // for Debugging memory leaks
}