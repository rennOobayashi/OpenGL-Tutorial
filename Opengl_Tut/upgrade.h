#ifndef UPGRADE_H
#define UPGRADE_H

#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "gameobject.h"
#include "texture.h"

const glm::vec2 size(60.0f, 20.0f);
const glm::vec2 velocity(0.0f, 100.0f);

class Upgrade : public GameObject
{
public:
	std::string Type;
	float duration;
	bool activated;

	Upgrade(std::string _type, glm::vec3 color, float _duration, glm::vec2 position, Texture texture)
		: GameObject(position, size, texture, color, velocity), Type(_type), duration(_duration), activated(false) { }
};

#endif