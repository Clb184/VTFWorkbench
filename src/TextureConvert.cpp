#include "TextureConvert.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "RootWindow.hpp"
#include "imgui_stdlib.h"
#include "Helper.hpp"
#include <assert.h>

VTFImageFormat ID2Format(int id) {
	switch(id) {
		case 0: return IMAGE_FORMAT_RGBA8888;
		case 1: return IMAGE_FORMAT_ABGR8888;
		case 2: return IMAGE_FORMAT_RGB888;
		case 3: return IMAGE_FORMAT_BGR888;
		case 4: return IMAGE_FORMAT_RGB565;
		case 5: return IMAGE_FORMAT_ARGB8888;
		case 6: return IMAGE_FORMAT_BGRA8888;
		case 7: return IMAGE_FORMAT_DXT1;
		case 8: return IMAGE_FORMAT_DXT3;
		default: case 9: return IMAGE_FORMAT_DXT5;
	}
}

const char* ID2Str(int id) {
	switch(id) {
		case IMAGE_FORMAT_RGBA8888: return "RGBA 8888";
		case IMAGE_FORMAT_ABGR8888: return "ABGR 8888";
		case IMAGE_FORMAT_RGB888: return "RGB 888";
		case IMAGE_FORMAT_BGR888: return "BGR 888";
		case IMAGE_FORMAT_RGB565: return "RGB 565";
		case IMAGE_FORMAT_ARGB8888: return "ARGB 8888";
		case IMAGE_FORMAT_BGRA8888: return "BGRA 8888";
		case IMAGE_FORMAT_DXT1: return "DXT1";
		case IMAGE_FORMAT_DXT3: return "DXT3";
		case IMAGE_FORMAT_DXT5: return "DXT5";
		default: return "Unknown";
	}
}

TextureConvert::TextureConvert(int id) {
	m_InternalName = "texture " + std::to_string(id); 
	// Set STB to flip image
	stbi_set_flip_vertically_on_load(false);
	memset(&m_CreateOptions, 0x00, sizeof(SVTFCreateOptions));
	m_CreateOptions.uiVersion[0] = 7;
	m_CreateOptions.uiVersion[1] = 2;
	m_CreateOptions.ImageFormat = IMAGE_FORMAT_DXT5;
	m_CreateOptions.uiFlags = 0;
	m_CreateOptions.uiStartFrame = 0;
	m_CreateOptions.sBumpScale = 1.0f;
	m_CreateOptions.bMipmaps = true;
	m_CreateOptions.MipmapFilter = MIPMAP_FILTER_BOX;
	m_CreateOptions.MipmapSharpenFilter = SHARPEN_FILTER_SHARPENSOFT;
	m_CreateOptions.bThumbnail = true;
	m_CreateOptions.bReflectivity = true;
	m_CreateOptions.bResize = true;
	m_CreateOptions.ResizeMethod = RESIZE_NEAREST_POWER2;
	m_CreateOptions.ResizeFilter = MIPMAP_FILTER_TRIANGLE;
	m_CreateOptions.ResizeSharpenFilter = SHARPEN_FILTER_NONE;
	m_CreateOptions.bResizeClamp = true;
	m_CreateOptions.uiResizeClampWidth = 8192;
	m_CreateOptions.uiResizeClampHeight = 8192;

	m_pPixelData = nullptr;
	m_bAvoidFree = true;
	m_Width = 0;
	m_Height = 0;
	m_TextureID = -1;
	m_TextureFormat = 8; 
	m_bFirstTime = true;
	m_X = 640.0f;
	m_Y = 64.0f;
}

TextureConvert::TextureConvert(int id, const wchar_t* filename) {
	m_InternalName = "texture " + std::to_string(id); 
	// Set STB to flip image
	stbi_set_flip_vertically_on_load(false);
	memset(&m_CreateOptions, 0x00, sizeof(SVTFCreateOptions));
	m_CreateOptions.uiVersion[0] = 7;
	m_CreateOptions.uiVersion[1] = 2;
	m_CreateOptions.ImageFormat = IMAGE_FORMAT_DXT5;
	m_CreateOptions.uiFlags = 0;
	m_CreateOptions.uiStartFrame = 0;
	m_CreateOptions.sBumpScale = 1.0f;
	m_CreateOptions.bMipmaps = true;
	m_CreateOptions.MipmapFilter = MIPMAP_FILTER_BOX;
	m_CreateOptions.MipmapSharpenFilter = SHARPEN_FILTER_SHARPENSOFT;
	m_CreateOptions.bThumbnail = true;
	m_CreateOptions.bReflectivity = true;
	m_CreateOptions.bResize = true;
	m_CreateOptions.ResizeMethod = RESIZE_NEAREST_POWER2;
	m_CreateOptions.ResizeFilter = MIPMAP_FILTER_TRIANGLE;
	m_CreateOptions.ResizeSharpenFilter = SHARPEN_FILTER_NONE;
	m_CreateOptions.bResizeClamp = true;
	m_CreateOptions.uiResizeClampWidth = 8192;
	m_CreateOptions.uiResizeClampHeight = 8192;
	
	wprintf(L"Load texture on constructor received: %s\n", filename);
	LoadTextureFromFile(filename);

	m_bAvoidFree = true;
	m_TextureFormat = 9; 
	
	// Loading window for first time
	m_bFirstTime = true;
	m_X = 640.0f;
	m_Y = 64.0f;
}

TextureConvert::~TextureConvert() {
	if(m_pPixelData && !m_bAvoidFree) {
		free(m_pPixelData);
		m_pPixelData = nullptr;
	}
}

bool TextureConvert::Move() {

	bool is_open = true;
	if(m_bFirstTime) {
		ImGui::SetNextWindowPos({m_X, m_Y});
		m_bFirstTime = false;
	}
	ImGui::Begin(m_InternalName.c_str(), &is_open, ImGuiWindowFlags_NoSavedSettings);
	// Test some groupings
	ImGui::BeginGroup();
	ImVec2 pos = ImGui::GetWindowPos();
	m_X = pos.x;
	m_Y = pos.y;
	//ImGui::Text("x %f, y %f", pos.x, pos.y);
	if(ImGui::Button("Load texture")) {
		bool on_success = false;
#ifdef WIN32
		COMDLG_FILTERSPEC filter;
		filter.pszName = L"Image files";
		filter.pszSpec = L"*.png; *.jpg; *.tga; *.bmp";

		on_success = CreateSingleSelectDialogWindows(&filter, 1, &m_InputName);
#endif
		if(on_success) {
			wprintf(L"Loading %s\n", m_InputName.c_str());
			if(nullptr != m_pPixelData) {
				free(m_pPixelData);
				glDeleteTextures(1, &m_TextureID);
			}
			LoadTextureFromFile(std::wstring(m_InputName.begin(), m_InputName.end()));
		}
		
	}
	ImGui::SameLine();
	ImGui::Text("%ls", m_InputName.c_str());
	ImGui::InputText("##output_image", &m_OutputName);
	ImGui::SameLine();
	if(ImGui::Button("Save file")){
		SaveFile(RootWindow::GetBasePath() / RootWindow::GetMaterialPath());
	}
	ImGui::EndGroup();

	ImGui::Image(m_TextureID, {256.0f, 256.0f});
	ImGui::SameLine();
	ImGui::BeginGroup();

	ImGui::BeginGroup();
	ImGui::CheckboxFlags("Point Sample", &m_CreateOptions.uiFlags, TEXTUREFLAGS_POINTSAMPLE);
	ImGui::CheckboxFlags("Trilinear", &m_CreateOptions.uiFlags, TEXTUREFLAGS_TRILINEAR);
	ImGui::CheckboxFlags("Clamp S", &m_CreateOptions.uiFlags, TEXTUREFLAGS_CLAMPS);
	ImGui::CheckboxFlags("Clamp T", &m_CreateOptions.uiFlags, TEXTUREFLAGS_CLAMPT);
	ImGui::CheckboxFlags("Anisotropic", &m_CreateOptions.uiFlags, TEXTUREFLAGS_ANISOTROPIC);
	ImGui::CheckboxFlags("Hint DXT5", &m_CreateOptions.uiFlags, TEXTUREFLAGS_HINT_DXT5);
	ImGui::CheckboxFlags("SRGB", &m_CreateOptions.uiFlags, TEXTUREFLAGS_SRGB);
	ImGui::CheckboxFlags("Normal", &m_CreateOptions.uiFlags, TEXTUREFLAGS_NORMAL);
	ImGui::CheckboxFlags("No MipMap", &m_CreateOptions.uiFlags, TEXTUREFLAGS_NOMIP);
	ImGui::CheckboxFlags("No LoD", &m_CreateOptions.uiFlags, TEXTUREFLAGS_NOLOD);
	ImGui::EndGroup();
	ImGui::SameLine();
		
	ImGui::BeginGroup();
	ImGui::CheckboxFlags("Min MipMap", &m_CreateOptions.uiFlags, TEXTUREFLAGS_MINMIP);
	ImGui::CheckboxFlags("Procedural", &m_CreateOptions.uiFlags, TEXTUREFLAGS_PROCEDURAL);
	//ImGui::CheckboxFlags("One bit Alpha", &m_CreateOptions.uiFlags, TEXTUREFLAGS_ONEBITALPHA);
	//ImGui::CheckboxFlags("Eight bit Alpha", &m_CreateOptions.uiFlags, TEXTUREFLAGS_EIGHTBITALPHA);
	//ImGui::CheckboxFlags("Environemnt map", &m_CreateOptions.uiFlags, TEXTUREFLAGS_ENVMAP);
	ImGui::CheckboxFlags("Render Target", &m_CreateOptions.uiFlags, TEXTUREFLAGS_RENDERTARGET);
	ImGui::CheckboxFlags("Depth Render Target", &m_CreateOptions.uiFlags, TEXTUREFLAGS_DEPTHRENDERTARGET);
	ImGui::CheckboxFlags("Single Copy", &m_CreateOptions.uiFlags, TEXTUREFLAGS_SINGLECOPY);
	ImGui::CheckboxFlags("No Depth Buffer", &m_CreateOptions.uiFlags, TEXTUREFLAGS_NODEPTHBUFFER);
	ImGui::CheckboxFlags("Clamp U", &m_CreateOptions.uiFlags, TEXTUREFLAGS_CLAMPU);
	ImGui::CheckboxFlags("Vertex texture", &m_CreateOptions.uiFlags, TEXTUREFLAGS_VERTEXTEXTURE);
	ImGui::CheckboxFlags("SS Bump", &m_CreateOptions.uiFlags, TEXTUREFLAGS_SSBUMP);
	ImGui::CheckboxFlags("Border", &m_CreateOptions.uiFlags, TEXTUREFLAGS_BORDER);
	ImGui::EndGroup();
	
	// For texture formats	
	const char* format_names[] = {
	       	"RGBA 8888", "ABGR 8888", "RGB 888", "BGR 888", "RGB 565",
	       	"ARGB 8888", "BGRA 8888", "DXT1", "DXT3", "DXT5"
       	};
	//bool in_combo = ImGui::BeginCombo("Format", ID2Str(m_TextureFormat));
	ImGui::SetNextItemWidth(96.0f);
	ImGui::Combo("Format", &m_TextureFormat, format_names, 10);
	/*for(int i = 0; i < 10; i++){
		ImGui::BeginChild(ID2Str(i));
		ImGui::EndChild();
	}*/
	
	//if(in_combo) ImGui::EndCombo();

	ImGui::EndGroup();
	

	ImGui::End();
	if(!is_open) {
		printf("closed window\n");
	}
	return is_open;
}

void TextureConvert::SetDelete() {
	m_bAvoidFree = false;
}

void TextureConvert::SetTextureFlags(int flags) {
	m_CreateOptions.uiFlags = flags;
}

void TextureConvert::SetTextureFormat(int format) {
	m_TextureFormat = format;
}

void TextureConvert::SaveFile(const std::filesystem::path& base_path) {
	if(nullptr != m_pPixelData) {
		// Normalize as remove any blank space before and after the text,
		// if there's nothing, skip
		NormalizeString(&m_OutputName);
		if(m_OutputName == "") return;

		// Set image format and create the VTF file
		m_CreateOptions.ImageFormat = ID2Format(m_TextureFormat);
		bool created = m_VTFFile.Create(m_Width, m_Height, m_pPixelData, m_CreateOptions);
		if(created) {
			printf("Converted to VTF (Format: %d)\n", ID2Format(m_TextureFormat));
			
			// Check for the material path
			RootWindow::CheckCreateMissingBasePath();
			
			// Join and make the final file path
			std::filesystem::path path = RootWindow::GetBasePath() / RootWindow::GetMaterialPath / std::filesystem::path(m_OutputName + ".vtf");
			printf("Path is: %s\n", path.string().c_str());
			printf("Filename is %s\n", path.filename().string().c_str());

			// Save file
			if(m_VTFFile.Save(path.string().c_str())) {
				printf("Saving converted VTF file\n");
			} else {
				printf("Error converting VTF file\n");
			}
		} else {
			printf("Error converting file: %s\n", vlGetLastError());
		}
	} else {
		printf("Attempted to save null data\n");
	}
}


const std::wstring TextureConvert::GetTextureSource() const {
	return m_InputName;
}

void TextureConvert::SetTextureName(const std::string& name) {
	m_OutputName = name;
}

const std::string TextureConvert::GetTextureName() const {
	return m_OutputName;
}

int TextureConvert::GetTextureFlags() {
	return m_CreateOptions.uiFlags;
}

int TextureConvert::GetTextureFormat() {
	return m_TextureFormat;
}

void TextureConvert::AsJSON(nlohmann::json* out) {
	assert(nullptr != out);
	try {
		nlohmann::json js;
		js["source"] = m_InputName;
		js["flags"] = m_CreateOptions.uiFlags;
		js["format"] = m_TextureFormat;
		js["name"] = m_OutputName;
		float pos[2] = {m_X, m_Y};
		js["pos"] = pos;

		*out = std::move(js);
	}
	catch(const std::exception& e){
		printf("JSON exception: %s\n", e.what());
	}
}

void TextureConvert::SetWindowPosition(float pos[2]) {
	m_X = pos[0];
	m_Y = pos[1];
	printf("Setting window pos at x %f, y %f\n", m_X, m_Y);
}

bool TextureConvert::LoadTextureFromFile(const std::wstring& filename) {
	bool on_success = false;
	int channel = 0;
	int w = 0, h = 0;
	uint8_t* pixels = nullptr;
	uint8_t* file_data = nullptr;
	std::ifstream image(filename.c_str(), std::ios::binary);
	size_t sz = 0;

	// Load file in memory and pass it to STB
	wprintf(L"Loading texture \"%s\"\n", filename.c_str());
	if(image.is_open()){
		wprintf(L"Image %s opened\n", filename.c_str());
		image.seekg(0, std::ios::end);
		sz = image.tellg(); // Get size in bytes to allocate
		printf("tellg returned %zu\n", sz);
		image.seekg(0, std::ios::beg);
		file_data = (uint8_t*)calloc(sz, 1);

		// Maybe we got no allocated memory or yes
		if(nullptr != file_data) {
			image.read((char*)file_data, sz);
			pixels = stbi_load_from_memory(file_data, sz, &w, &h, &channel, 4);
		}

		// Maybe we didn't get image data or yes
		if(nullptr != pixels) {
			wprintf(L"Loaded texture \"%s\" (w: %d h: %d channels: %d)\n", filename.c_str(), w, h, channel);
			on_success = true;
		}
		if(nullptr != file_data) {
			free(file_data);
		}
	}
	if(false == on_success) {
		wprintf(L"Failed loading texture \"%s\"\n", filename.c_str());
		m_Width = 256;
		m_Height = 256;
		m_pPixelData = (uint8_t*)calloc(256 * 256, 4);
		return false;
	}
	

	
	// Fine, I'll do it myself (Manual forced alpha or other channels)
	uint8_t* ndata = (uint8_t*)calloc(w * h, 4);
	
	// Since STB can handle the channel conversion, I'll leave it to them
	memcpy(ndata, pixels, w * h * 4);

	// Now the texture itself
	GLuint tex = 0;
	glCreateTextures(GL_TEXTURE_2D, 1, &tex);
	glTextureStorage2D(tex, 1, GL_RGBA32F, w, h);
	glTextureSubImage2D(tex, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	stbi_image_free(pixels);

	m_pPixelData = ndata;
	m_Width = w;	
	m_Height = h;
	m_TextureID = tex;
	
	size_t size = filename.length();
	int start = 0;
	for(size_t i = 0; i < size; i++){
		if(filename.at(i) == '\\' || filename.at(i) == '/'){
			start = i + 1;
		}
	}
	m_OutputName = "";
	for(int i = size - 1; i > start; i--) {
		if(filename.at(i) == '.') {
			for(int c = start; c < i; c++){
				m_OutputName.push_back(filename.at(c));
			}
			break;
		}
	}
	m_InputName = filename;
	return true;
}
