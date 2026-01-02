#include "gameobject.h"

GameObject::GameObject() 
	: position(0.0f), size(1.0f), velocity(0.0f), color(1.0f), rotation(0.0f), sprite(), is_solid(false), destroyed(false), flipX(false), flipY(false) {}

GameObject::GameObject(glm::vec2 _position, glm::vec2 _size, Texture _sprite, glm::vec3 _color, glm::vec2 _velocirty) 
	: position(_position), size(_size), velocity(_velocirty), color(_color), rotation(0.0f), sprite(_sprite), is_solid(false), destroyed(false), flipX(false), flipY(false) { }

void GameObject::draw(SpriteRenderer &renderer) {
	glm::vec2 obj_position = position;
	glm::vec2 obj_size = size;

	if (flipX) {
		obj_size.x = -obj_size.x;
		obj_position.x += obj_size.x;
	}
	
	if (flipY) {
		obj_size.y = -obj_size.y;
		obj_position.y += obj_size.y;
	}

	renderer.draw_sprite(sprite, obj_position, obj_size, rotation, color);
}