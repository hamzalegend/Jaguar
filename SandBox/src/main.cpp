#include "Core/Application.h"
#include "SandBoxLayer.h"

int main()
{
	Application app;
	Ref<SandBoxLayer> sandBoxLayer = MakeRef<SandBoxLayer>();
	app.PushLayer(sandBoxLayer);
	app.Run();
	return 0;
}