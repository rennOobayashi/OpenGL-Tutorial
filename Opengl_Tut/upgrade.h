#ifndef UPGRADE_H
#define UPGRADE_H

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "gameobject.h"

const glm::vec2 Size(60.0f, 20.0f);
const glm::vec2 Velocity(0.0f, 100.0f);

class Upgrade : public GameObject
{
public:
	std::string type;
	float duration;
	bool activated;

	Upgrade(std::string _type, glm::vec3 color, float _duration, glm::vec2 position, Texture texture)
		: GameObject(position, Size, texture, color, Velocity), type(_type), duration(_duration), activated(false) { }
};

#endif