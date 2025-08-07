#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "spriterenderer.h"
#include "shader.h"

class PostProcessor
{
private:
	unsigned int msfbo, fbo;
	unsigned int rbo;
	unsigned int vao;

	void init_render_data();
public:
	Shader shader;
	Texture texture;
	unsigned int width, height;
	bool confuse, chaos, shake;
	
	PostProcessor(Shader _shader, unsigned int _width, unsigned int _height);
	void begin_render();
	void end_render();
	void render(float time);
};

#endif