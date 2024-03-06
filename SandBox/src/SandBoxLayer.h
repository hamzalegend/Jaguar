#pragma once

#include "Layer/Layer.h"

#include "Scene/Scene.h"
#include "Renderer/Mesh.h"

#include <vector>
#include <API/FrameBuffer.h>

#include "Scene/Scene.h"
#include <Serialization/SceneSerializer.h>

class SandBoxLayer : public Layer
{
public:
	SandBoxLayer();
	~SandBoxLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(float DeltaTime) override;
	virtual void OnUIUpdate();

private:
	Ref<Scene> m_scene = nullptr;
	Ref<AssetManager> m_assetManager = MakeRef<AssetManager>();

	Ref<SceneSerializer> serializer = nullptr;

};
