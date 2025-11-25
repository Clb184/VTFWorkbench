#ifndef ROOTWINDOW_INCLUDED
#define ROOTWINDOW_INCLUDED

#include "TextureConvert.hpp"
#include "MaterialConstructor.hpp"

#include <vector>
#include <filesystem>

#include "GLFW/glfw3.h"

#define VTFW_VERSION "1.2.0"

struct output_vmt_t {
	std::string name;
	int base_texture = 0;
	int template_material = 0;
};

void DropFileProc(GLFWwindow* window, int cnt, const char* paths[]);

class RootWindow {
public:
	RootWindow(float width, float height);
	~RootWindow();

	// Logic
	void Move();
	void HandleDroppedFiles(int cnt, const char* paths[]);

	// Manage paths
	static const std::filesystem::path GetBasePath();
	static const std::filesystem::path GetMaterialPath();
	static void CheckCreateMissingBasePath();

private:
	// Move the path and outputs mixer
	void MoveBaseVars();
	void MoveMaterialOutputs();

	// Move the texture and material windows
	void MoveTexConvert();
	void MoveMaterialConstructors();

	// Texture convert menu
	void OpenTextureDialog();
	
	// Material template menu	
	void OpenMaterialTemplateDialog();
	void CreateMaterialConstructor();

	// Manage material outputs
	bool SaveMaterial(const output_vmt_t& output);
	void RemoveTextureFromOutputs(size_t id);
	void RemoveMaterialFromOutputs(size_t id);
	
	// Manage project
	void OpenProject();
	void SaveProject();
	void ClearWorkspace();

	// Load stuff from file
	void LoadTextureFromFile(const wchar_t* filename);
	void LoadMaterialTemplateFromFile(const wchar_t* filename);
	//void LoadProjectFromFile(const wchar_t* ); Might screw up work, so I'm not gonna consider it

private:
	bool m_bOpenFirstTime;
	float m_XPath, m_YPath;
	float m_XOutput, m_YOutput;
	float m_Width;
	float m_Height;
	int m_TexConvID;
	int m_MatConstID;
	std::vector<TextureConvert> m_CvtInstances;
	std::vector<MaterialConstructor> m_MatCInstances;

	// Combine material templates and base textures
	std::vector<output_vmt_t> m_OutputsList;

	// Paths
	static std::string m_BasePath;
	static std::string m_MaterialPath;
};

#endif
