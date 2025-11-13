#ifndef TEXTURECONVERT_INCLUDED
#define TEXTURECONVERT_INCLUDED

#include "VTFLib.h"
#include "imgui.h"
#include "GL/glew.h"
#include <stdint.h>
#include <string>

// Texture convert window and its contents
class TextureConvert {
public:
	TextureConvert(int id);
	~TextureConvert();

	bool Move();
	bool ConvertToVTF(VTFImageFormat format);
	void SetDelete();
private:
	bool LoadTextureFromFile();

private:
	std::string m_InternalName;
	char m_InputName[1024 + 1];
	char m_OutputName[1024 + 1];
	bool m_bAvoidFree;
	uint8_t* m_pPixelData;
	uint32_t m_Width;
	uint32_t m_Height;
	GLuint m_TextureID;
	VTFLib::CVTFFile m_VTFFile;
	SVTFCreateOptions m_CreateOptions;
};

#endif
