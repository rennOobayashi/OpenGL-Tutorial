#include "textrenderer.h"

TextRenderer::TextRenderer() {
	init();
}

void TextRenderer::init() {

	if (FT_Init_FreeType(&ft)) {
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}

	if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}
}