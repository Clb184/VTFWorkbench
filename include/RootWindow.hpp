#ifndef ROOTWINDOW_INCLUDED
#define ROOTWINDOW_INCLUDED

#include "TextureConvert.hpp"
#include <vector>

class RootWindow {
public:
	RootWindow(float width, float height);
	~RootWindow();

	void Move();

private:
	void MoveTexConvert();

private:
	float m_Width;
	float m_Height;

	int m_TexConvID;
	std::vector<TextureConvert> m_CvtInstances;
};

#endif
