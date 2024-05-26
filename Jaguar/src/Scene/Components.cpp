#include "Components.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"

#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtx/euler_angles.hpp"

glm::mat4 TransformComponent::GetTransform() const
{
	glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

	return glm::translate(glm::mat4(1.0f), Position)
		* rotation
		* glm::scale(glm::mat4(1.0f), Scale);
}

glm::mat4 TransformComponent::GetGlobalTransform() const
{
	if (Parent == 0) return GetTransform();

	TransformComponent& ParentTC = m_Scene->GetEntityByUUID(Parent).GetComponent<TransformComponent>();

	return ParentTC.GetGlobalTransform() * GetTransform();
}

glm::vec3 TransformComponent::GetGlobalPosition() const
{
	if (Parent == 0) return Position;
	return Position + m_Scene->GetEntityByUUID(Parent).GetComponent<TransformComponent>().GetGlobalPosition();
}

glm::vec3 TransformComponent::GetGlobalRotation() const
{
	if (Parent == 0) return Rotation;
	return Rotation;
		//+ m_Scene->GetEntityByUUID(Parent).GetComponent<TransformComponent>().GetGlobalRotation();
}
glm::vec3 TransformComponent::GetGlobalScale() const
{
	if (Parent == 0) return Scale;
	return Scale * m_Scene->GetEntityByUUID(Parent).GetComponent<TransformComponent>().GetGlobalScale();
}
void TransformComponent::SetGlobalTransform(glm::mat4 mat)
{
	glm::vec3 position, scale;
	glm::quat rotation;
	glm::vec3 skew;
	glm::vec4 perspective;




	glm::decompose(glm::inverse(GetGlobalTransform() * GetTransform()) * mat, scale, rotation, position, skew, perspective);

	Position = position;//local
	Rotation = glm::eulerAngles(rotation);//local
	Scale = scale; //local
};


void TransformComponent::SetGlobalPosition(const glm::vec3& pos) {
	if (Parent == 0)
	{
		Position = pos;
		return;
	}

	// Get the global transform of the parent
	const auto& parentTransform = m_Scene->GetEntityByUUID(Parent).GetComponent<TransformComponent>().GetGlobalTransform();

	// Calculate the local position relative to the parent
	glm::mat4 parentInverse = glm::inverse(parentTransform);
	glm::vec4 localPos = parentInverse * glm::vec4(pos, 1.0f);

	// Set the local position
	Position = glm::vec3(localPos);
}

void TransformComponent::SetGlobalRotation(const glm::vec3& rot) {
	if (Parent == 0)
	{
		Rotation = rot;
		return;
	}

	// Get the global transform of the parent
	const auto& parentTransform = m_Scene->GetEntityByUUID(Parent).GetComponent<TransformComponent>().GetGlobalTransform();

	// Extract the rotation part from the parent's global transform
	glm::mat3 parentRotationMatrix = glm::mat3(parentTransform);
	glm::quat parentRotation = glm::quat_cast(parentRotationMatrix);

	// Calculate the local rotation relative to the parent
	glm::quat globalRotQuat = glm::quat(glm::radians(rot));
	glm::quat localRotQuat = glm::inverse(parentRotation) * globalRotQuat;

	// Convert the local rotation quaternion to Euler angles
	Rotation = glm::degrees(glm::eulerAngles(localRotQuat));
}


void TransformComponent::SetGlobalScale(const glm::vec3& scl) {
	if (Parent == 0)
	{
		Scale = scl;
		return;
	}

	// Get the global transform of the parent
	const auto& parentTransform = m_Scene->GetEntityByUUID(Parent).GetComponent<TransformComponent>().GetGlobalTransform();

	// Extract the scale part from the parent's global transform
	glm::vec3 parentScale = glm::vec3(glm::length(glm::vec3(parentTransform[0])),
		glm::length(glm::vec3(parentTransform[1])),
		glm::length(glm::vec3(parentTransform[2])));

	// Calculate the local scale relative to the parent
	Scale = scl / parentScale;
}