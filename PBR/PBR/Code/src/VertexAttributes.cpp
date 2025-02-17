#include "VertexAttributes.h"

VertexAttributes::VertexAttributes()
{
	m_VAO = 0;
}

VertexAttributes::~VertexAttributes()
{
	glDeleteVertexArrays(1, &m_VAO);
}

void VertexAttributes::Load()
{
	glGenVertexArrays(1, &m_VAO);
}

void VertexAttributes::Bind() const
{
	glBindVertexArray(m_VAO);
}

void VertexAttributes::Setup()
{
#define POSITION 0
	glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, pos)));
	glEnableVertexAttribArray(POSITION);
#undef POSITION
#define NORMAL 1
	glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(NORMAL);
#undef NORMAL
#define UV 2
	glVertexAttribPointer(UV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));
	glEnableVertexAttribArray(UV);
#undef UV
/*#define TANGENT 3
	glVertexAttribPointer(TANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tangent)));
	glEnableVertexAttribArray(TANGENT);
#undef TANGENT
#define BITANGENT 4
	glVertexAttribPointer(BITANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, bitangent)));
	glEnableVertexAttribArray(BITANGENT);
#undef BITANGENT*/
}

void VertexAttributes::Reset()
{
	glBindVertexArray(0);
}

void VertexAttributes::Unload() const
{
	glDeleteVertexArrays(1, &m_VAO);
}
