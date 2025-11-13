#include "RootWindow.hpp"

RootWindow::RootWindow(float width, float height) 
	: m_Width(width), m_Height(height) 
{
	m_TexConvID = 0;
}

RootWindow::~RootWindow() {

}

void RootWindow::Move() {
	// Window itself
	ImGui::SetNextWindowPos({0.0f, 0.0f});
	ImGui::SetNextWindowSize({m_Width, m_Height});
	ImGui::Begin("vtftool_root", nullptr,
	       	ImGuiWindowFlags_NoCollapse |
	       	ImGuiWindowFlags_NoResize |
	       	ImGuiWindowFlags_NoMove |
	       	ImGuiWindowFlags_MenuBar |
	       	ImGuiWindowFlags_NoTitleBar |
	       	ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoSavedSettings
		);

	// The menu bar where some useful stuff is
	ImGui::BeginMainMenuBar();
	if(ImGui::MenuItem("Load Texture")) {
		printf("Load texture\n");
		m_CvtInstances.emplace_back(m_TexConvID);
		m_TexConvID++;
	}
	ImGui::EndMainMenuBar();
	
	// Texture convert
	MoveTexConvert();

	// End main window
	ImGui::End();

}

void RootWindow::MoveTexConvert() {
	for (uint32_t i = 0; i < m_CvtInstances.size(); i++) {
		// Delete from list if closes
		if(!m_CvtInstances[i].Move()) {
			m_CvtInstances.erase(m_CvtInstances.begin() + i);
			i--;
		}
	}
}
