#include "Menu.h"
#include "Application.h"

static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::BeginItemTooltip())
	{
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

Menu::Menu(Application* application)
{
	app = application; 
	
	isUsingTexture = app->IsUsingTexture();
	
	albedoCol = app->GetAlbedo();
	lightCol = app->GetLightColor();
	lightPosition = app->GetLightPosition(0);

	LightInt = app->GetLightIntensity();
	metallic = app->GetMetallic();
	roughness = app->GetRoughness();
	ambientOcclusion = app->GetAmbientOcclusion();
}

void Menu::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Menu::Init(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	const ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.Fonts->AddFontDefault();
	mainFont = io.Fonts->AddFontFromFileTTF("Assets/Fonts/helvetica-rounded-bold.otf", 24.0f, nullptr, io.Fonts->GetGlyphRangesDefault());
	IM_ASSERT(mainFont != nullptr);

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");
}


void Menu::Render(const Camera& camera)
{
	// Init imgui window
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//Rendering
	Draw(camera); // Render all custom windows
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Menu::Draw(const Camera& camera)
{
	const ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;

	ImGui::PushFont(mainFont);

	ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
	ImGui::SetNextWindowSize(ImVec2(0.f, 0.f));
	if (ImGui::Begin("PBR Settings", nullptr, window_flags))
	{
		ImGui::SetWindowFontScale(0.7f);
		if (ImGui::BeginTabBar("Main Bar"))
		{
			if (ImGui::BeginTabItem("APP SETTINGS"))
			{
				if (ImGui::CollapsingHeader("Show Settings"))
				{
					ImGui::Dummy(ImVec2(15, 15));
					// Object Tree Window
					if (ImGui::Checkbox("Use Object Textures", &isUsingTexture))
						app->SetUsingTexture(isUsingTexture);

					MainSettings();
					DebugInfo(camera);
				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("HELP MENU"))
			{
				ImGui::SetWindowFontScale(1.f);
				ImGui::TextColored(ImVec4(255.f, 0.f, 0.f, 255.f),
				                   "Here is a list of all the available input settings :\n\n");
				ImGui::TextColored(ImVec4(0.686f, 0.658f, 0.003f, 1.f), "WASD :");
				ImGui::TextColored(ImVec4(0.f, 100.f, 255.f, 255.f), "Move around the scene\n\n");
				ImGui::TextColored(ImVec4(0.686f, 0.658f, 0.003f, 1.f), "LEFT SHIFT :");
				ImGui::TextColored(ImVec4(0.f, 255.f, 80.f, 255.f), "Go Down\n\n");
				ImGui::TextColored(ImVec4(0.686f, 0.658f, 0.003f, 1.f), "SPACE BAR :");
				ImGui::TextColored(ImVec4(0.f, 100.f, 255.f, 255.f), "Go Up\n\n");
				ImGui::TextColored(ImVec4(0.686f, 0.658f, 0.003f, 1.f), "RIGHT CLICK (HOLD) AND MOVE THE MOUSE :");
				ImGui::TextColored(ImVec4(0.f, 100.f, 255.f, 255.f), "Rotate the camera\n\n");
				ImGui::TextColored(ImVec4(0.686f, 0.658f, 0.003f, 1.f), "ESCAPE :");
				ImGui::TextColored(ImVec4(0.f, 100.f, 255.f, 255.f), "Close the app\n\n");
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}
	ImGui::End();
	ImGui::PopFont();
}

void Menu::MainSettings()
{
	ImGui::Dummy(ImVec2(30.f, 30.f));
	ImGui::SeparatorText("LIGHT COLOR");
	ImGui::PushItemWidth(375);
	ImGui::ColorPicker3("##01", reinterpret_cast<float*>(&lightCol), ImGuiColorEditFlags_DisplayRGB);
	app->SetLightColor(lightCol);

	ImGui::Dummy(ImVec2(20.f, 20.f));
	HelpMarker("CTRL + LEFT CLICK : Manually input a value"); ImGui::SameLine(); ImGui::SeparatorText("LIGHT INTENSITY");
	ImGui::SliderFloat("##02", &LightInt, 0.f, 100.f, "LIGHT INTENSITY : %.3f");
	app->SetLightIntensity(LightInt);

	ImGui::Dummy(ImVec2(15.f, 15.f));
	HelpMarker("CTRL + LEFT CLICK : Manually input a value"); ImGui::SameLine(); ImGui::SeparatorText("LIGHT POSITIONS");
	ImGui::PushItemWidth(200.f);
	ImGui::SliderFloat("##031", &lightPosition.x, -25.f, 25.f, "X : %.2f"); ImGui::PushItemWidth(200.f); ImGui::SameLine();
	ImGui::SliderFloat("##032", &lightPosition.y, -25.f, 25.f, "Y : %.2f"); ImGui::PushItemWidth(200.f); ImGui::SameLine();
	ImGui::SliderFloat("##033", &lightPosition.z, -25.f, 25.f, "Z : %.2f");
	app->lightPositions[0] = lightPosition;

	if (isUsingTexture)
		ImGui::BeginDisabled();
		
	ImGui::Dummy(ImVec2(50.f, 50.f));
	HelpMarker("CTRL + LEFT CLICK : Manually input a value"); ImGui::SameLine(); ImGui::SeparatorText("ALBEDO COLOR");
	ImGui::PushItemWidth(375);
	ImGui::ColorPicker3("##04", reinterpret_cast<float*>(&albedoCol), ImGuiColorEditFlags_DisplayRGB);
	app->SetAlbedo(normalize(albedoCol));
	
	ImGui::Dummy(ImVec2(25.f, 25.f));
	
	ImGui::PushItemWidth(500);
	HelpMarker("CTRL + LEFT CLICK : Manually input a value"); ImGui::SameLine(); ImGui::SliderFloat("##05", &metallic, 0.01f, 1.f, "Metallic : %.3f");
	app->SetMetallic(metallic);

	ImGui::PushItemWidth(500);
	HelpMarker("CTRL + LEFT CLICK : Manually input a value"); ImGui::SameLine(); ImGui::SliderFloat("##06", &roughness, 0.0899f, 1.f, "Roughness : %.3f");
	ImGui::PushItemWidth(150);
	app->SetRoughness(roughness);

	ImGui::PushItemWidth(500);
	HelpMarker("CTRL + LEFT CLICK : Manually input a value"); ImGui::SameLine(); ImGui::SliderFloat("##07", &ambientOcclusion, 0.f, 1.f, "Ambient Occlusion : %.3f");
	app->SetAmbientOcclusion(ambientOcclusion);

	if (isUsingTexture)
		ImGui::EndDisabled();
}

void Menu::DebugInfo(const Camera& camera) const
{
	ImGui::Dummy(ImVec2(35.f, 35.f));
	ImGui::BulletText("Camera Position :");
	ImGui::Indent(); ImGui::Text("x : %f   |   y : %f   |   z : %f \n Pitch : %f   |   Yaw : %f", camera.Position.x, camera.Position.y, camera.Position.z, camera.GetPitch(), camera.GetYaw()); ImGui::SameLine(); ImGui::Unindent();
}
