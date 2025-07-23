#include "ballobject.h"

Ball::Ball() : GameObject(), radius(12.5f), stuck(true) {}

Ball::Ball(glm::vec2 position, float radius, glm::vec2 velocity, Texture sprite)
	: GameObject(position, glm::vec2(radius * 2.0f), sprite, glm::vec3(1.0f), velocity), radius(radius), stuck(true) { }

glm::vec2 Ball::move(float dt, unsigned int window_width) {
	if (!stuck) {
		//bounce ball
		position += velocity * dt;

		//Check if the wall collides width the windows on the left and right
		//and bounces back the other way.
		if (position.x <= 0.0f) {
			velocity.x = -velocity.x;
			position.x = 0.0f;
		}
		else if (position.x + size.x >= window_width) {
			velocity.x = -velocity.x;
			position.x = 0.0f;
		}
		//Check if the wall collides width the windows on the top
		if (position.y <= 0.0f) {
			velocity.y = velocity.y;
			position.y = 0.0f;
		}
	}

	return position;
}

void Ball::reset(glm::vec2 position, glm::vec2 velocity) {
	this->position = position;
	this->velocity = velocity;
	stuck = true;
}