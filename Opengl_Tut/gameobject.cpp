#include "gameobject.h"

GameObject::GameObject() 
	: position(0.0f), size(1.0f), velocity(0.0f), color(1.0f), rotation(0.0f), sprite(), is_solid(false), destroyed(false) {}

GameObject::GameObject(glm::vec2 _position, glm::vec2 _size, Texture _sprite, glm::vec3 _color, glm::vec2 _velocirty) 
	: position(_position), size(_size), velocity(_velocirty), color(_color), rotation(0.0f), sprite(_sprite), is_solid(false), destroyed(false) { }

void GameObject::draw(SpriteRenderer &renderer) {
	renderer.draw_sprite(sprite, position, size, rotation, color);
}