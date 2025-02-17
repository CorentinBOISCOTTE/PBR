#include "PBRObject.h"

PBRObject::PBRObject(Shader _shader, Mesh _mesh, Mesh _texturedMesh, vec3 _albedo, float _metallic, float _roughness, float _ao, Texture _albedoMap, Texture _normalMap, Texture _metallicMap, Texture _roughnessMap, Texture _aoMap, bool _textured)
{
	shader = _shader;
	mesh = _mesh;
	texturedMesh = _texturedMesh;
	albedo = _albedo;
	metallic = _metallic;
	roughness = _roughness;
	ao = _ao;
	albedoMap = _albedoMap;
	normalMap = _normalMap;
	metallicMap = _metallicMap;
	roughnessMap = _roughnessMap;
	aoMap = _aoMap;
	textured = _textured;
}

void PBRObject::Load()
{
	mesh.Load();
	texturedMesh.Load();
	albedoMap.Load();
	normalMap.Load();
	metallicMap.Load();
	roughnessMap.Load();
	aoMap.Load();
}

void PBRObject::Unload()
{
	mesh.Unload();
	texturedMesh.Unload();
	albedoMap.Unload();
	normalMap.Unload();
	metallicMap.Unload();
	roughnessMap.Unload();
	aoMap.Unload();
}

void PBRObject::Draw() const
{
	shader.use();

	shader.setBool("textured", textured);
	if (textured)
	{
		mat4 model = mat4(1.f);
		vec3 scale = vec3(0.01f, 0.01f, 0.01f);
		model = glm::translate(model, glm::vec3(0.f));
		model = rotate(model, glm::radians(0.f), vec3(0.f, 1.f, 0.f));
		//model = glm::scale(model, scale);
		shader.setMat4("model", model);
		shader.setMat3("normalMatrix", transpose(inverse(mat3(model))));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, albedoMap.textureID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normalMap.textureID);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, metallicMap.textureID);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, roughnessMap.textureID);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, aoMap.textureID);

		glBindVertexArray(texturedMesh.attributes.m_VAO);
		glDrawElements(GL_TRIANGLES, texturedMesh.nbIndices, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}
	else
	{
		mat4 model = mat4(1.f);
		vec3 scale = vec3(0.01f, 0.01f, 0.01f);
		model = translate(model, vec3(0.f));
		model = rotate(model, radians(0.f), vec3(0.f, 1.f, 0.f));
		model = glm::scale(model, scale);
		shader.setMat4("model", model);
		shader.setMat3("normalMatrix", transpose(inverse(mat3(model))));
		
		shader.setVec3("a", albedo);
		shader.setFloat("m", metallic);
		shader.setFloat("r", roughness);
		shader.setFloat("ambientOcclusion", ao);

		glBindVertexArray(mesh.attributes.m_VAO);
		glDrawElements(GL_TRIANGLES, mesh.nbIndices, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}
}

void PBRObject::Update(const vec3 _albedo, const float _metallic, const float _roughness, const float _ao, const bool _textured)
{
	albedo = _albedo;
	metallic = _metallic;
	roughness = _roughness;
	ao = _ao;
	textured = _textured;
}