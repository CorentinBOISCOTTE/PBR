#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "glad/glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera;
using namespace glm;

class Application;

class Menu
{
public:
	Menu(Application* application);
	~Menu() = default;

	void Destroy();

	void Init(GLFWwindow* window);
	void Render(const Camera& camera);
	void Draw(const Camera& camera);

	void MainSettings();
	void DebugInfo(const Camera& camera) const;

private:
	Application* app = nullptr;

	vec3 albedoCol;
	vec3 lightCol;
	vec3 lightPosition;

	float LightInt;
	float metallic;
	float roughness;
	float ambientOcclusion;

	bool isUsingTexture = false;

	ImFont* mainFont = nullptr;
};