#ifndef GAME_H
#define GAME_H

#include "spriterenderer.h"
#include "resourcemanager.h"
#include "gamelevel.h"
#include "texture.h"
#include "ballobject.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <random>
#include <fstream>
#include <iostream>
#include <vector>

enum GameState {
	GAME_ACTIVE, //0
	GAME_MENU,   //1
	GAME_WIN     //2
};

enum Direction {
	UP, 
	DOWN, 
	LEFT, 
	RIGHT
};

typedef std::tuple<bool, Direction, glm::vec2> Collision;

class Game 
{
private:
	GameState states;
	unsigned int width, height;
	unsigned int tex1;
	float delta_time;
	float last_frame;
	float speed;

	GLFWwindow* window;
	SpriteRenderer* renderer;
	std::vector<GameLevel> levels;
	unsigned int level;

	GameObject *player;
	Ball* ball;

	Collision check_collision(Ball &ball, GameObject &object);

	Direction vector_direction(glm::vec2 target);
public:
	Game(unsigned int _width, unsigned int _height);
	~Game(); //Destructor
	void init();
	void process_input(GLFWwindow* window, float dt);
	void update();
	void render();
	void do_collisions();
};

#endif