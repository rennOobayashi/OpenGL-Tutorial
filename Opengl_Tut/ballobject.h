#ifndef BALLOBJECT_H
#define BALLOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "gameobject.h"
#include "texture.h"

class Ball : public GameObject
{
public:
	float radius;
	bool stuck;

	Ball();
	Ball(glm::vec2 position, float radius, glm::vec2 velocity, Texture sprite);

	glm::vec2 move(float dt, unsigned int window_width, float speed);
	void reset(glm::vec2 position, glm::vec2 velocity);
};

#endif 