#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GL/glew.h"
#include "glfw/glfw3.h"
#include "RootWindow.hpp"
#include <fstream>
#include <string>

void TestDropPath(GLFWwindow* window, int count, const char* paths[]) {
	std::wofstream path_out("test_output_path.txt", std::ios::out);
	if(path_out.is_open()) {
		for(int i = 0; i < count; i++) {
			size_t size;
			wchar_t buffer[1024] = L"";
			mbstowcs_s(&size, buffer, 1024, paths[i], 1024);
			path_out << buffer << L"\n";
			wprintf(L"Path %s dropped into window\n", buffer);
			std::filesystem::path path(buffer);
			wprintf(L"Filename: %s, Extension: %s", path.filename().wstring().c_str(), path.extension().wstring().c_str());
		}
	}
}

int main() {
	
	printf("Initializing VTF Workbench\n");

	// Init GLFW
	printf("Initializing GLFW\n");
	if(!glfwInit()) {
		printf("Failed initializing GLFW\n");
	       	return -1;
	}

	// Setup for GL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// Set size of 720p
	int winw = 1280, winh = 720;
	// Create window
	printf("Creating GLFWwindow\n");
	GLFWwindow* window = glfwCreateWindow(winw, winh, "VTF Workbench", NULL, NULL);
	if(nullptr == window) { std::cout << "Failed creating window\n"; return -1; }
	glfwSetDropCallback(window, DropFileProc);

	glfwMakeContextCurrent(window);

	printf("Initializing GLEW\n");
	if(GLEW_OK != glewInit()) {
		printf("Failed initializing GLEW\n");
		return -1;
	}
	glfwSwapInterval(1);
	
	printf("GPU Vendor is: %s\n", glGetString(GL_VENDOR));
	printf("OpenGL version is: %s\n", glGetString(GL_VERSION));

	printf("Initializing ImGUI\n");
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();
	// Initialize ImGUI for OpenGL
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// Configure viewport
	glViewport(0, 0, winw, winh);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	char buf[1024] = "";
	char output[1024] = "";
	

	// Initialize VTFLib
	printf("Initializing VTFLib\n");
	vlInitialize();
	RootWindow* root_window = new RootWindow((float)winw, (float)winh);
	if(nullptr == root_window) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}
	glfwSetWindowUserPointer(window, root_window);
	while(!glfwWindowShouldClose(window)){
		// Process messages, we wait since we don't really do any animations
		glfwWaitEvents();
		glfwPollEvents();

		// New ImGUI frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		// Move logic
		root_window->Move();
		
		// Render contents and draw
		ImGui::Render();
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Move the Swap Chain
		glfwSwapBuffers(window);
	}
	//if(texconv)
	delete root_window;

	// Terminate all
	glfwDestroyWindow(window);
	window = nullptr;
	std::cout << "Ending program...\n";
	glfwTerminate();
}
