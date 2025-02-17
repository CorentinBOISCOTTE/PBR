#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Mesh.h"

class Object
{
public:
	Object(Shader _shader, Mesh _mesh);
	void Load();
	void Unload();
	void Draw();
	void Update(vec3 _pos, vec3 _color);

private:
	Shader shader{};
	Mesh mesh{};
	vec3 pos, color;

};