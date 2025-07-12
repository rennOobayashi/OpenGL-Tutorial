#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "spriterenderer.h"
#include "gameobject.h"
#include "resourcemanager.h"

class GameLevel
{
private:
	void init(std::vector<std::vector<unsigned int>> tile_data, unsigned int level_width, unsigned int level_height);

public:
	std::vector<GameObject> bricks;

	GameLevel() {}
	void load(const char *file, unsigned int level_width, unsigned int level_height);
	void draw(SpriteRenderer &renderer);
	bool is_completed();
};

#endif