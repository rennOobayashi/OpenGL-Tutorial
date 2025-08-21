#include "textrenderer.h"

TextRenderer::TextRenderer(unsigned  int _width, unsigned int _height) {
	init(_width, _height);
}

TextRenderer::~TextRenderer() {
	if (face) {
		FT_Done_Face(face);
	}
	if (ft) {
		FT_Done_FreeType(ft);
	}
}

void TextRenderer::init(unsigned  int _width, unsigned int _height) {
	tshader = ResourceManager::load_shader("geofragver/text_2d_vertex.vs", "geofragver/text_2d_fragment.fs", nullptr, "text");
	tshader.use().set_mat4("projection", glm::ortho(0.0f, float(_width), float(_height), 0.0f), true);
	tshader.set_int("text", 0);

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TextRenderer::load(std::string font, unsigned int font_size) {
	characters.clear();
	FT_Library ft;
	FT_Face face;

	if (FT_Init_FreeType(&ft)) {
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}

	if (FT_New_Face(ft, font.c_str(), 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}

	FT_Set_Pixel_Sizes(face, 0, font_size);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte c = 0; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
			continue;
		}
		unsigned int texture;

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 
			face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, 
			GL_UNSIGNED_INT, face->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,														 //id
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows), //size
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),   //bearing
			face->glyph->advance.x											 //advance
		};
	}
}