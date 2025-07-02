#include "spriterenderer.h"

void spriterenderer::init_render_data() {
	unsigned int vbo;

	//two triangles to make a quad
	float vertices[] = {
		//pos       // tex
		0.0f, 1.0f, 0.0f, 1.0f, //left top
		1.0f, 0.0f, 1.0f, 0.0f, //right bottom
		0.0f, 0.0f, 0.0f, 0.0f, //left bottom

		0.0f, 1.0f, 0.0f, 1.0f, //left top
		1.0f, 1.0f, 1.0f, 1.0f, //right top
		1.0f, 0.0f, 1.0f, 0.0f  //right bottom
	};

	glGenVertexArrays(1, &qao);
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(qao);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void spriterenderer::draw_sprite(texture& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color) {
	shader.use();

	glm::mat4 model = glm::mat4(1.0f);
	//It seems to indicate the initial starting point location.
	model = glm::translate(model, glm::vec3(position, 0.0f));

	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3( - 0.5f * size.x, -0.5f * size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));

	shader.set_mat4("model", model);
	shader.set_vec3("sprite_color", color);

	glActiveTexture(GL_TEXTURE0);
	texture.bind();

	glBindVertexArray(qao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}