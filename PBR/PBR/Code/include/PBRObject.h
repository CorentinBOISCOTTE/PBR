#pragma once
#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class PBRObject
{
public:
	PBRObject(Shader _shader, Mesh _mesh, Mesh _texturedMesh, vec3 _albedo, float _metallic, float _roughness, float _ao, Texture _albedoMap, Texture _normalMap, Texture _metallicMap, Texture _roughnessMap, Texture _aoMap, bool _textured);
	void Load();
	void Unload();
	void Draw() const;
	void Update(vec3 _albedo, float _metallic, float _roughness, float _ao, bool _textured);

private:
	Shader shader{};
	Mesh mesh{};
	Mesh texturedMesh{};
	Texture albedoMap, normalMap, metallicMap, roughnessMap, aoMap;
	vec3 albedo = vec3(0.f);
	float metallic;
	float roughness;
	float ao;
	bool textured;
};