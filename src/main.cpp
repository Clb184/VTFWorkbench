#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GL/glew.h"
#include "glfw/glfw3.h"

int main() {
	// Init GLFW
	if(!glfwInit()) return -1;

	// Setup for GL 4.5
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

	// Create window
	GLFWwindow* window = glfwCreateWindow(800, 600, "test", NULL, NULL);
	if(nullptr == window) { std::cout << "Failed creating window\n"; return -1; }

	glfwMakeContextCurrent(window);
	glewInit();
	glfwSwapInterval(1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();
	// Initialize ImGUI for OpenGL
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// Configure viewport
	glViewport(0, 0, 800, 600);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	char buf[256] = " ";

	while(!glfwWindowShouldClose(window)){
		// Process messages
		glfwPollEvents();

		// New ImGUI frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		// Window itself
		ImGui::SetNextWindowPos({0.0f, 0.0f});
		ImGui::SetNextWindowSize({800.0f, 600.0f});
		ImGui::Begin("vtftool_root", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar);
		
		// Test some groupings
		ImGui::BeginGroup();
		//ImGui::Text("texture");
	       	//ImGui::SameLine();
		ImGui::InputText("texture", buf, 256);
		ImGui::EndGroup();

		//ImGui::BeginGroup();
		//ImGui::EndGroup();
		
		// End main window
		ImGui::End();

		// Render contents and draw
		ImGui::Render();
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Move the Swap Chain
		glfwSwapBuffers(window);
	}

	// Terminate all
	glfwDestroyWindow(window);
	window = nullptr;
	std::cout << "Ending program...\n";
	glfwTerminate();
}
