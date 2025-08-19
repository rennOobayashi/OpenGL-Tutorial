#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>

class TextRenderer
{
private:
	FT_Library ft;
	FT_Face face;

	void init();
public:
	TextRenderer();
	~TextRenderer();
};

#endif