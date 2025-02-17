#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "Vertex.h"

class VertexAttributes
{
public:
	VertexAttributes();
	~VertexAttributes();
	void Load();
	void Bind() const;
	void Setup();
	void Reset();
	void Unload() const;
	GLuint m_VAO;
};