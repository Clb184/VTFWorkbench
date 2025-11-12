#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GL/glew.h"
#include "glfw/glfw3.h"
#include <assert.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "VTFLib.h"

GLuint LoadTexture(const char* filename, uint32_t* pw, uint32_t* ph, uint8_t** pixel_data) {
	assert(nullptr != pixel_data);
	int w = 0, h = 0;
	int channel = 0;
	uint8_t* pixels = nullptr;
	if(!(pixels = stbi_load(filename, &w, &h, &channel, 0))) {
		printf("Failed loading texture \"%s\"\n", filename);
		return -1;
	}

	printf("Loaded texture \"%s\" (w: %d h: %d)\n", filename, w, h);
	
	GLuint tex = 0;
	glCreateTextures(GL_TEXTURE_2D, 1, &tex);
	glTextureStorage2D(tex, 1, GL_RGBA32F, w, h);
	glTextureSubImage2D(tex, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	*pixel_data = pixels;
	if(pw){ *pw = w; }	
	if(ph){ *ph = h; }
	return tex;
}

int main() {

	VTFLib::CVTFFile vtf;
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
	
	// Set STB to flip image
	stbi_set_flip_vertically_on_load(false);

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
	char buf[1024] = "";
	char output[1024] = "";
	
	GLuint misaka = 0; // image id for OpenGL
	uint32_t iw = 0, ih = 0; // width and height
	uint8_t* pixel_data = nullptr;

	// Initialize VTFLib
	vlInitialize();

	int image_flags = TEXTUREFLAGS_SRGB | TEXTUREFLAGS_TRILINEAR;

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
		ImGui::Begin("vtftool_root", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);
		ImGui::Begin("texture");
		// Test some groupings
		ImGui::BeginGroup();
		//ImGui::Text("texture");
	       	//ImGui::SameLine();
		ImGui::InputText("##input_image", buf, 1024);
		ImGui::SameLine();
		if(ImGui::Button("Load texture")) {
			printf("%s\n", buf);
			if(nullptr != pixel_data) stbi_image_free(pixel_data);
			misaka = LoadTexture(buf, &iw, &ih, &pixel_data);
			size_t size = strlen(buf);
			for(int i = size - 1; i > 0; i--){
				if(buf[i] == '.') {
				memcpy(output, buf, i);
				if(size < 1024 - 4) {
					output[i] = '.';
					output[i + 1] = 'v';
					output[i + 2] = 't';
					output[i + 3] = 'f';
				}
				break;
				}
			}
		}
		ImGui::SameLine();
		if(ImGui::Button("VTF")) {
			if(nullptr != pixel_data) {
				SVTFCreateOptions options = {0};
				options.uiVersion[0] = 7;
				options.uiVersion[1] = 2;
				
				options.ImageFormat = IMAGE_FORMAT_DXT5;
				options.uiFlags = image_flags;
				options.uiStartFrame = 0;
				options.sBumpScale = 1.0f;
				options.bMipmaps = true;
				options.MipmapFilter = MIPMAP_FILTER_BOX;
				options.MipmapSharpenFilter = SHARPEN_FILTER_SHARPENSOFT;
				options.bThumbnail = true;
				options.bReflectivity = true;
				options.bResize = true;
				options.ResizeMethod = RESIZE_NEAREST_POWER2;
				options.ResizeFilter = MIPMAP_FILTER_TRIANGLE;
				options.ResizeSharpenFilter = SHARPEN_FILTER_NONE;
				options.bResizeClamp = true;
				options.uiResizeClampWidth = 8192;
				options.uiResizeClampHeight = 8192;


				bool created = vtf.Create(iw, ih, pixel_data, options);
				if(created) {
					printf("Converted to VTF\n");
				} else {
					printf("Error converting file: %s", vlGetLastError());
				}
			} else {
				printf("Attempted to load null image data\n");
			}
		}
		ImGui::InputText("##output_image", output, 1024);
		ImGui::SameLine();
		if(ImGui::Button("Save file")){
			if(nullptr != pixel_data) {
				vtf.Save(output);
				printf("Saving converted VTF file\n");
			} else {
				printf("Attempted to save null data\n");
			}
		}
		ImGui::EndGroup();
		ImGui::Image(misaka, {256.0f, 256.0f});
		ImGui::CheckboxFlags("Point Sample", &image_flags, TEXTUREFLAGS_POINTSAMPLE);
		ImGui::CheckboxFlags("Trilinear", &image_flags, TEXTUREFLAGS_TRILINEAR);
		ImGui::CheckboxFlags("Clamp S", &image_flags, TEXTUREFLAGS_CLAMPS);
		ImGui::CheckboxFlags("Clamp T", &image_flags, TEXTUREFLAGS_CLAMPT);
		ImGui::CheckboxFlags("Anisotropic", &image_flags, TEXTUREFLAGS_ANISOTROPIC);
		ImGui::CheckboxFlags("Hint DXT5", &image_flags, TEXTUREFLAGS_HINT_DXT5);
		ImGui::CheckboxFlags("SRGB", &image_flags, TEXTUREFLAGS_SRGB);
		//ImGui::BeginGroup();
		//ImGui::EndGroup();
		
		ImGui::End();
		// End main window
		ImGui::End();

		// Render contents and draw
		ImGui::Render();
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Move the Swap Chain
		glfwSwapBuffers(window);
	}
	if(pixel_data) stbi_image_free(pixel_data);
	// Terminate all
	glfwDestroyWindow(window);
	window = nullptr;
	std::cout << "Ending program...\n";
	glfwTerminate();
}
