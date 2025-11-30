#ifndef TEXTURECONVERT_INCLUDED
#define TEXTURECONVERT_INCLUDED

#include "VTFLib.h"
#include "imgui.h"
#include "GL/glew.h"
#include <stdint.h>
#include <string>
#include <filesystem>
#include <fstream>
#include "json.hpp"

// Texture convert window and its contents
class TextureConvert {
public:
	// Constructors and Destructor
	TextureConvert(int id);
	TextureConvert(int id, const wchar_t* filename);
	~TextureConvert();

	// Default stuff
	bool Move();
	void SetDelete();
	void SaveFile(const std::filesystem::path& base_path);

	// Get some stuff
	const std::wstring GetTextureSource() const;
	void SetTextureName(const std::string& name);
	const std::string GetTextureName() const;
	int GetTextureFlags();
	int GetTextureFormat();
	void SetTextureFlags(int flags);
	void SetTextureFormat(int format);
	void SetWindowPosition(float pos[2]);
	
	// A quick format to save info in the savefile
	void AsJSON(nlohmann::json* out);

private:
	bool LoadTextureFromFile(const std::wstring& filename);

private:
	bool m_bFirstTime;
	float m_X, m_Y;
	std::string m_InternalName;
	std::wstring m_InputName;
	std::string m_OutputName;
	bool m_bAvoidFree;
	uint8_t* m_pPixelData;
	uint32_t m_Width;
	uint32_t m_Height;
	int m_TextureFormat;
	GLuint m_TextureID;
	VTFLib::CVTFFile m_VTFFile;
	SVTFCreateOptions m_CreateOptions;
};

#endif
