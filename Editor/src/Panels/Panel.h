#pragma once
#include "Engine.h"

#include "Camera/EditorCamera.h"
#include <ImGuizmo/ImGuizmo.h>

class Panel
{
public:
	Panel();
	~Panel();

	virtual void Draw()=0;

private:

};
