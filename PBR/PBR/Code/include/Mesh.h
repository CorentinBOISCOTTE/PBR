#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Buffer.h"
#include "VertexAttributes.h"

//#include "assimp/Importer.hpp"
//#include "scene.h"
//#include "postprocess.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct aiMesh;

class Mesh
{
public:
	Mesh() = default;
	Mesh(const std::string& _filename);
	void Load();
	void Unload();
	uint16_t nbIndices;
	VertexAttributes attributes;
private:
	void BindBuffers(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
	Buffer buffer;
	std::string filename;
	void ProcessMesh(const aiMesh* mesh);
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	bool isLoaded = false;
};