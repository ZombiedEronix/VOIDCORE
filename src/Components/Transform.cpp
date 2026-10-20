#include "Transform.h"

Transform::Transform()
	: Position(0.0f), Rotation(0.0f), Scale(1.0f) {
}

void Transform::SetDirty()
{
	isDirty = true;
	for (Transform* child : children)
	{
		child->SetDirty();
	}
}

void Transform::SetPosition(const glm::vec3& pos)
{
	Position = pos;
	SetDirty();
}

void Transform::SetRotation(const glm::vec3& rot)
{
	Rotation = rot;
	SetDirty();
}

void Transform::SetScale(const glm::vec3& scl)
{
	Scale = scl;
	SetDirty();
}



void Transform::SetParent(Transform* prnt)
{
	if (parent)
	{
		auto& siblings = parent->children;
		siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
	}

	parent = prnt;

	if (parent)
	{
		parent->children.push_back(this);
	}

	SetDirty();
}

const Transform* Transform::GetParent() 
{
	return parent;
}

const std::vector<Transform*>& Transform::GetChildren()
{
	return children;
}

const glm::vec3& Transform::GetPosition()
{
	return Position;
}

const glm::vec3& Transform::GetRotation()
{
	return Rotation;
}

const glm::vec3& Transform::GetScale()
{
	return Scale;
}

glm::mat4 Transform::WorldMatrix() const
{
	if (isDirty)
	{
		glm::mat4 local = LocalMatrix();
		if (parent)
		{
			cachedWorldMatrix = parent->WorldMatrix() * local;
		}
		else
		{
			cachedWorldMatrix = local;
		}
		isDirty = false;
	}
	return cachedWorldMatrix;
}

glm::mat4 Transform::LocalMatrix() const
{
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, Position);
	model = glm::rotate(model, glm::radians(Rotation.y), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(Rotation.x), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(Rotation.z), glm::vec3(0, 0, 1));
	model = glm::scale(model, Scale);

	return model;
}
glm::vec3 Transform::GetForward() const
{
	//Fix!
	float pitch = glm::radians(Rotation.x);
	float yaw   = glm::radians(Rotation.y);

	glm::vec3 forward;
	forward.x = cos(yaw) * cos(pitch);
	forward.y = sin(pitch);
	forward.z = sin(yaw) * cos(pitch);

	return glm::normalize(forward);
}

glm::vec3 Transform::GetRight() const
{
	glm::vec3 forward = GetForward();
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	
	return glm::normalize(glm::cross(forward, worldUp));
}

glm::vec3 Transform::GetUp() const
{
	return glm::normalize(glm::cross(GetRight(), GetForward()));
}
