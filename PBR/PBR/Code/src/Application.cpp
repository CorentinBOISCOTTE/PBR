#include "Application.h"

static float deltaTime = 0.0f;
static float lastFrame = 0.0f;

void Application::Init(const uint16_t width, const uint16_t height)
{
	m_width = width;
	m_height = height;

	lastPos = vec2(m_width * 0.5f, m_height * 0.5f);

	lightPositions = {
		glm::vec3(0.0f, 0.0f, 10.f)
	};

	if (!glfwInit())
		return;

	window = glfwCreateWindow(m_width, m_height, "PBR Shaders", nullptr, nullptr);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	if (!window)
	{
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << '\n';
		return;
	}

	menu = new Menu(this);
	menu->Init(window);

	printf("GL_VENDOR: %p\n", glGetString(GL_VENDOR));
	printf("GL_VERSION: %p\n", glGetString(GL_VERSION));
	printf("GL_RENDERER: %p\n", glGetString(GL_RENDERER));
}

void Application::Update()
{
	glEnable(GL_DEPTH_TEST);

	Shader shader("Assets/Shaders/shader.vert", "Assets/Shaders/shader.frag");
	Shader lightObjectShader("Assets/Shaders/lightObject.vert", "Assets/Shaders/lightObject.frag");

    camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	Mesh trashMesh("Assets/Meshes/trash.obj");
	Mesh highResSphere("Assets/Meshes/3d-model.fbx");
	Texture albedoMap("Assets/Textures/trash bag_uw_BaseColor.png");
	Texture normalMap("Assets/Textures/trash bag_uw_BaseColor.png");
	Texture metallicMap("Assets/Textures/trash bag_uw_Metallic.png");
	Texture roughnessMap("Assets/Textures/trash bag_uw_Roughness.png");
	Texture aoMap("Assets/Textures/trash bag_uw_AO.png");

	PBRObject sphere(shader, highResSphere, trashMesh, albedo, metallic, roughness, ambientOcclusion, albedoMap, normalMap, metallicMap, roughnessMap, aoMap, usingTexture);
	Object pointLight(lightObjectShader, highResSphere);
	sphere.Load();
	pointLight.Load();


	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		CloseWindowInput();
		CameraInput();

		shader.use();
		shader.setVec3("lightColors", glm::normalize(lightColor));

		for (vec3 lightPos : lightPositions)
			shader.setVec3("lightPositions", lightPos);
		
		shader.setVec3("camPos", camera.Position);
		shader.setFloat("roughness", roughness);
		shader.setFloat("lightIntensity", lightIntensity);

		int width = m_width, height = m_height;
		glfwGetFramebufferSize(window, &width, &height);
		UpdateProjection(shader, width, height);

		UpdateView(shader);

		glClearColor(0.467f, 0.71f, 1.f, 0.996f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		sphere.Update(albedo, metallic, roughness, ambientOcclusion, usingTexture);
		sphere.Draw();

		lightObjectShader.use();

		const mat4 projection = perspective(radians(camera.Zoom), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
		lightObjectShader.setMat4("projection", projection);

		const mat4 view = camera.GetViewMatrix();
		lightObjectShader.setMat4("view", view);

		pointLight.Update(lightPositions[0], lightColor);
		pointLight.Draw();

		menu->Render(camera);

		glfwSwapBuffers(window);
		ResizeWindowOnStart();
	}

	pointLight.Unload();
	sphere.Unload();
}

void Application::Terminate() const
{
	menu->Destroy();
	glfwTerminate();
}

void Application::UpdateProjection(const Shader shader, const int width, const int height) const
{
	const mat4 projection = perspective(radians(camera.Zoom), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
	shader.setMat4("projection", projection);
}

void Application::UpdateView(const Shader shader) const
{
	const mat4 view = camera.GetViewMatrix();
	shader.setMat4("view", view);
}

void Application::CloseWindowInput() const
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void Application::CameraInput()
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);

	double xPosIn, yPosIn;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glfwGetCursorPos(window, &xPosIn, &yPosIn);

		const float xpos = static_cast<float>(xPosIn);
		const float ypos = static_cast<float>(yPosIn);

		const float xoffset = xpos - lastPos.x;
		const float yoffset = lastPos.y - ypos; // reversed since y-coordinates go from bottom to top

		lastPos.x = xpos;
		lastPos.y = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwGetCursorPos(window, &xPosIn, &yPosIn);
		lastPos = vec2(xPosIn, yPosIn);
	}
}

void Application::ResizeWindowOnStart()
{
	if (wasWindowResized)
		return;
	
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
	GLFWvidmode* nMode = const_cast<GLFWvidmode*>(videoMode);
	
	glfwSetWindowSize(window, videoMode->width, videoMode->height);
	
	glfwGetWindowFrameSize(window, nullptr, nullptr, &nMode->width, &nMode->height);
	glfwSetWindowPos(window, 0, videoMode->height + (nMode->height * 3));

	wasWindowResized = true;
}

Camera Application::GetCamera() const
{
	return camera;
}

void Application::SetLightIntensity(const float value)
{
	lightIntensity = value;
}

void Application::SetMetallic(const float value)
{
	metallic = value;
}

void Application::SetRoughness(const float value)
{
	roughness = value;
}

void Application::SetAmbientOcclusion(const float value)
{
	ambientOcclusion = value;
}

void Application::SetAlbedo(const vec3 value)
{
	albedo = value;
}

void Application::SetLightColor(const vec3 value)
{
	lightColor = value;
}

void Application::SetUsingTexture(const bool value)
{
	usingTexture = value;
}

float Application::GetLightIntensity() const
{
	return lightIntensity;
}

float Application::GetMetallic() const
{
	return metallic;
}

float Application::GetRoughness() const
{
	return roughness;
}

float Application::GetAmbientOcclusion() const
{
	return ambientOcclusion;
}

vec3 Application::GetAlbedo() const
{
	return albedo;
}

vec3 Application::GetLightColor() const
{
	return lightColor;
}

vec3 Application::GetLightPosition(const int index) const
{
	return lightPositions[index];
}

bool Application::IsUsingTexture() const
{
	return usingTexture;
}

void FramebufferSizeCallback(GLFWwindow* window, const int width, const int height)
{
	glViewport(0, 0, width, height);
	glScissor(0, 0, width, height);
}
