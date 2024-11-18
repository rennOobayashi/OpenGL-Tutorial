#ifndef MESH_H
#define MESH_H
#include "shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

//vertex attribute�� ã�µ� �ʿ��� ����
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoords;
};

struct Texture {
	unsigned int texture;
	std::string type; //diffuse�� specular ���� ��
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


