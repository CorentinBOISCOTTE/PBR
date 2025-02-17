#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Mesh.h"
#include "PBRObject.h"
#include "Menu.h"
#include "Object.h"

#include <cstdint>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>"
#include <vector>


class Application
{
public:
	Application() = default;
	~Application() = default;
	void Init(uint16_t width, uint16_t height);
	void Terminate() const;
	void Update();

	void SetLightIntensity(float value);
	void SetMetallic(float value);
	void SetRoughness(float value);
	void SetAmbientOcclusion(float value);
	void SetAlbedo(vec3 value);
	void SetLightColor(vec3 value);
	void SetUsingTexture(bool value);

	float GetLightIntensity() const;
	float GetMetallic() const;
	float GetRoughness() const;
	float GetAmbientOcclusion() const;
	vec3 GetAlbedo() const;
	vec3 GetLightColor() const;
	vec3 GetLightPosition(int index) const;
	bool IsUsingTexture() const;

	void UpdateProjection(Shader shader, int width, int height) const;
	void UpdateView(Shader shader) const;

	Camera GetCamera() const;

	std::vector<vec3> lightPositions{};

private:
	uint16_t m_width = 1920;
	uint16_t m_height = 1080;
	GLFWwindow* window = nullptr;
	Menu* menu;
	Camera camera{};

	float lightIntensity = 50.f;
	float metallic = 0.5f;
	float roughness = 0.5f;
	float ambientOcclusion = 0.5f;

	vec3 albedo = vec3(1.f);
	vec3 lightColor = vec3(1.f);

	vec2 lastPos{};

	bool usingTexture = false;
	bool wasWindowResized = false;

	void CloseWindowInput() const;
	void CameraInput();
	void ResizeWindowOnStart();
};

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
