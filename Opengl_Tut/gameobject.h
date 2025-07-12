#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "spriterenderer.h"

class GameObject
{
public:
	glm::vec2 position, size, velocity;
	glm::vec3 color;
	float rotation;
	bool is_solid;
	bool destroyed;
	Texture sprite;
	
	GameObject();
	GameObject(glm::vec2 _position, glm::vec2 _size, Texture _sprite, glm::vec3 _color = glm::vec3(1.0f), 
		glm::vec2 _velocirty = glm::vec2(0.0f));
	//Can be override
	virtual void draw(SpriteRenderer &renderer);
};

#endif