#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <map>

#include "resourcemanager.h"
#include "texture.h"
#include "shader.h"

struct Character
{
	unsigned int texture_id;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;
};

class TextRenderer
{
private:
	std::map<char, Character> characters;
	Shader tshader;
	unsigned int vao, vbo;

	void init(unsigned int _width, unsigned int _height);
public:
	TextRenderer(unsigned int _width, unsigned int _height);
	~TextRenderer();
	void load(std::string font, unsigned int font_size);
	void render_text(std::string text, float x, float y, float scale, glm::vec2 color = glm::vec3(1.0f));
};

#endif