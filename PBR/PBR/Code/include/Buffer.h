#pragma once
#include "glad/glad.h"

#include <vector>
#include "Vertex.h"

class Buffer
{
public:
	Buffer();
	~Buffer();
	void Load();
	void Bind(const std::vector<Vertex>& vertexBuffer, const std::vector<uint32_t>& indexBuffer);
	void Reset();

private:
	GLuint m_VBO;
	GLuint m_EBO;
	std::vector<Vertex> m_vertexBuffer;
	std::vector<uint32_t> m_indexBuffer;
};