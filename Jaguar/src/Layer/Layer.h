#pragma once

#include "Core/DEFINES.h"

class JAGUAR_API Layer
{
public:
	Layer();
	virtual ~Layer();

	virtual void OnAttach();
	virtual void OnDetach();
	virtual void OnUpdate(float DeltaTime);

	virtual void OnUIUpdate();
	

	inline void SetEnabled(bool enabled) { m_enabled = enabled; }
	inline bool IsEnabled() { return m_enabled; }

private:
	bool m_enabled = true;
};