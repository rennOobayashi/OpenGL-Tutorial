#ifndef MESH_H
#define MESH_H
#include "shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

//vertex attribute를 찾는데 필요한 벡터
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoords;
};

struct Texture {
	unsigned int texture;
	std::string type; //diffuse나 specular 같은 거
};

class Mesh
{
private:
	unsigned int vao, vbo, ebo;
	void setup_mesh();

public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void draw(Shader shader);
};

#endif // !MESH_H


