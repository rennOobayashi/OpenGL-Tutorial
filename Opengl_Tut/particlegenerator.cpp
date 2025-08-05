#include "particlegenerator.h"

ParticleGenerator::ParticleGenerator(Shader _shader, Texture _texture, unsigned int _amount) 
	: shader(_shader), texture(_texture), amount(_amount) {

	init();
}

void ParticleGenerator::init() {
	unsigned int pbo;
	float particle_quad[] = {
		0.0f, 1.0f, 0.0f, 1.0f, // top left
		1.0f, 0.0f, 1.0f, 0.0f, // bottom right
		0.0f, 0.0f, 0.0f, 0.0f, // bottom left

		0.0f, 1.0f, 0.0f, 1.0f, // top left
		1.0f, 1.0f, 1.0f, 1.0f, // top right
		1.0f, 0.0f, 1.0f, 0.0f, // bottom right
	};

	glGenVertexArrays(1, &pao);
	glGenBuffers(1, &pbo);
	glBindVertexArray(pao);
	glBindBuffer(GL_ARRAY_BUFFER, pbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	for (unsigned int i = 0; i < amount; ++i) {
		particles.push_back(Particle());
	}
}

void ParticleGenerator::update(float dt, GameObject& object, unsigned int new_particles, glm::vec2 offset) {
	//add new particle
	for (unsigned int i = 0; i < new_particles; ++i) {
		int unused_particle = first_unused_particle();
		respawn_particle(particles[unused_particle], object, offset);
	}

	//particle update
	for (unsigned int i = 0; i < amount; ++i) {
		Particle& par = particles[i];
		par.life -= dt;

		if (par.life > 0.0f) {
			par.position -= par.velocity * dt;
			par.color.a -= dt * 2.5f; //fade out
		}
	}
}

//return the index of the first unused particle
unsigned int ParticleGenerator::first_unused_particle() {
	//saerch from the last used particle
	//this will be faster than searching from the beginning
	for (unsigned int i = last_used_particle; i < amount; ++i) {
		if (particles[i].life <= 0.0f) {
			last_used_particle = i;
			return i;
		}
	}

	//if not found, search from the beginning
	for (unsigned int i = 0; i < last_used_particle; ++i) {
		if (particles[i].life <= 0.0f) {
			last_used_particle = i;
			return i;
		}
	}

	//if all particles are used, reset to 0
	last_used_particle = 0;
	return 0;
}

void ParticleGenerator::respawn_particle(Particle& particle, GameObject& object, glm::vec2 offset) {
	float random = ((rand() % 100) - 50) / 7.0f;
	float rcolor = 0.7f + ((rand() % 100) / 100.0f);
	particle.position = object.position + random + offset;
	particle.color = glm::vec4(rcolor, rcolor, rcolor, 1.0f);
	particle.life = 0.15f;
	particle.velocity = object.velocity * 0.1f;
}

void ParticleGenerator::draw() {
	//GL_SRC_ALPHA is used to blend the particles with the background
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	shader.use();

	for (Particle particle : particles) {
		if (particle.life > 0.0f) {
			shader.set_vec2("offset", particle.position);
			shader.set_vec4("color", particle.color);
			texture.bind();
			glBindVertexArray(pao);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}