#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1600, 900, "Tetris!", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "ERROR!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = { 0.3f,0.3f,0.3f,1.0f };
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 150");

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		Renderer::Clear();
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		float deltaTime = ImGui::GetIO().DeltaTime;

		ImGui::Begin("Left Panel");
		ImGui::SetWindowPos({ 0.0f, -30.0f });
		ImGui::SetWindowSize({ 500.0f, 930.0f });
		ImGui::SetWindowFontScale(1.5f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", deltaTime * 1000, ImGui::GetIO().Framerate);

		ImGui::End();

		ImGui::Begin("Right Panel");
		ImGui::SetWindowPos({ 1100.0f, -30.0f });
		ImGui::SetWindowSize({ 500.0f, 930.0f });
		ImGui::SetWindowFontScale(1.5f);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}