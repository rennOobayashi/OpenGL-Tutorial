#ifndef PARTICLEGENERATOR_H
#define PARTICLEGENERATOR_H
#include <vector>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "shader.h"
#include "texture.h"
#include "gameobject.h"

struct Particle {
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec4 color;
	float life;

	Particle() : position(0.0f), velocity(0.0f), color(1.0f), life(0.0f) {}
};

const int nr_particles = 500;

class ParticleGenerator
{
private:
	std::vector<Particle> particles;
	Shader shader;
	Texture texture;
	unsigned int amount;
	unsigned int pao;
	unsigned int last_used_particle = 0;
	void init();
	unsigned int first_unused_particle();
	void respawn_particle(Particle &particle, GameObject &object, glm::vec2 offset = glm::vec2(0.0f));


public:
	ParticleGenerator(Shader _shader, Texture _texture, unsigned int _amount);
	void update(float dt, GameObject &object, unsigned int new_particles, glm::vec2 offset = glm::vec2(0.0f));
	void draw();
};

#endif