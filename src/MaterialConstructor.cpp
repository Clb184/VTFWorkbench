#include "MaterialConstructor.hpp"
#include "imgui_stdlib.h"

MaterialConstructor::MaterialConstructor(int id) {
	m_InternalName = "material " + std::to_string(id);
	m_pNodes = nullptr;
}

MaterialConstructor::~MaterialConstructor() {

}

bool MaterialConstructor::Move() {
	bool is_open = true;
	ImGui::Begin(m_InternalName.c_str(), &is_open, ImGuiWindowFlags_NoSavedSettings);


	DrawAddButtons();
	DrawNodeValues();
	ImGui::End();
	if(!is_open) {
		printf("Closing Material Constructor window\n");
	}
	return is_open;
}

void MaterialConstructor::DrawAddButtons() {
	ImGui::BeginGroup();
	// To add values of different kinds
	if(ImGui::Button("Add integer")) {
		m_Nodes.emplace_back(NODE_INTEGER);
	}
	ImGui::SameLine();
	if(ImGui::Button("Add float")) {
		m_Nodes.emplace_back(NODE_FLOAT);
	}
	ImGui::SameLine();
	if(ImGui::Button("Add float 2")) {
		m_Nodes.emplace_back(NODE_FLOAT2);
	}
	ImGui::SameLine();
	if(ImGui::Button("Add float 3")) {
		m_Nodes.emplace_back(NODE_FLOAT3);
	}
	ImGui::SameLine();
	if(ImGui::Button("Add float 4")) {
		m_Nodes.emplace_back(NODE_FLOAT4);
	}
	ImGui::SameLine();
	if(ImGui::Button("Add string")) {
		m_Nodes.emplace_back(NODE_STRING);
	}
	ImGui::SameLine();
	if(ImGui::Button("Add color")) {
		m_Nodes.emplace_back(NODE_COLOR);
	}
	ImGui::EndGroup();
	//ImGui::SameLine();
}

void MaterialConstructor::DrawNodeValues() {
	ImGui::BeginGroup();
	for(int i = 0; i < m_Nodes.size(); i++){
		std::string name = "##name" + std::to_string(i);
		std::string value = "##value" + std::to_string(i);
		ImGui::SetNextItemWidth(96.0f);
		ImGui::InputText(name.c_str(), &m_Nodes[i].name);
		ImGui::SameLine();
		switch(m_Nodes[i].type){
			case NODE_INTEGER: ImGui::InputInt(value.c_str(), &m_Nodes[i].integer); break;
			case NODE_FLOAT: ImGui::InputFloat(value.c_str(), &m_Nodes[i].single); break;
			case NODE_FLOAT2: ImGui::InputFloat2(value.c_str(), m_Nodes[i].float2); break;
			case NODE_FLOAT3: ImGui::InputFloat3(value.c_str(), m_Nodes[i].float3); break;
			case NODE_FLOAT4: ImGui::InputFloat4(value.c_str(), m_Nodes[i].float4); break;
			case NODE_COLOR: ImGui::ColorEdit4(value.c_str(), m_Nodes[i].float4); break;
			case NODE_STRING: ImGui::InputText(value.c_str(), &m_Nodes[i].string); break;
		}		
	}
	ImGui::EndGroup();
}
