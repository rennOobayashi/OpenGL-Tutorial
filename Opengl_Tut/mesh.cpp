#include "mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setup_mesh();
}

void Mesh::setup_mesh() {
	glGenBuffers(1, &vao);

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	//vertex normals
	//offsetof(struct, struct variable): struct의 start부터 입력된 변수까지의 바이트 offset return
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)); 

	//vertex textrue coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoords));

	glBindVertexArray(0);

}

void Mesh::draw(Shader shader) {
	unsigned int diffuse_nr = 1;
	unsigned int specular_nr = 1;

	for (unsigned int i = 0; i < textures.size(); i++) {
		std::string number;
		std::string name;

		glActiveTexture(GL_TEXTURE0 + i);

		name = textures[i].type;
		//if texture type is diffuse
		if (name == "texture_diffuse") { 
			number = std::to_string(diffuse_nr++);
		}
		//if texture type is specular
		else if (name == "texture_specular") {
			number = std::to_string(specular_nr++);
		}
		//example: material.diffuse1
		shader.set_float("material." + name + number.c_str(), i);

		glActiveTexture(GL_TEXTURE0);
	}

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}