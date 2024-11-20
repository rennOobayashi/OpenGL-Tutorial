#ifndef MESH_H
#define MESH_H
#include "shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#define MAX_BONE_INFLUENCE 4

//vertex attribute를 찾는데 필요한 벡터
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoords;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	int m_bone_ids[MAX_BONE_INFLUENCE];
	float m_weights[MAX_BONE_INFLUENCE];
};

struct Texture {
	unsigned int id;
	std::string type; //diffuse나 specular 같은 거
	std::string path;
};

class Mesh
{
private:
	unsigned int vbo, ebo;
	void setup_mesh();

public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	unsigned int vao;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void draw(Shader& shader);
};

#endif // !MESH_H


