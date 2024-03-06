#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera() {};
	~Camera() {};

	virtual glm::mat4& GetProjection() { return m_Projection; };
	void SetProjection(glm::mat4 p) { m_Projection = p; };
	float FOV = 45;
	float NearPlane = 0.1f;
	float FarPlane = 100.0f;
private:
	glm::mat4 m_Projection;

};
