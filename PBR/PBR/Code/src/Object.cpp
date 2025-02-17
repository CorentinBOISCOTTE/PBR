#include "Object.h"

Object::Object(Shader _shader, Mesh _mesh)
{
	shader = _shader;
	mesh = _mesh;
}

void Object::Load()
{
	mesh.Load();
}

void Object::Unload()
{
	mesh.Unload();
}

void Object::Draw()
{
	shader.use();

	mat4 model = mat4(1.f);
	vec3 scale = vec3(0.001f, 0.001f, 0.001f);
	model = glm::translate(model, pos);
	model = rotate(model, glm::radians(0.f), vec3(0.f, 1.f, 0.f));
	model = glm::scale(model, scale);
	shader.setMat4("model", model);
	shader.setVec3("color", color);

	glBindVertexArray(mesh.attributes.m_VAO);
	glDrawElements(GL_TRIANGLES, mesh.nbIndices, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void Object::Update(vec3 _pos, vec3 _color)
{
	pos = _pos;
	color = _color;
}
