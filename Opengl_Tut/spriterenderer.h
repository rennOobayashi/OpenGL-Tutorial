#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"

class SpriteRenderer
{
private:
	Shader shader;
	unsigned int qao;

	void init_render_data();
public:
	SpriteRenderer(Shader &shader);
	~SpriteRenderer();

	void draw_sprite(Texture &texture, glm::vec2 position, 
		glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
		glm::vec3 color = glm::vec3(1.0f));
};

#endif
