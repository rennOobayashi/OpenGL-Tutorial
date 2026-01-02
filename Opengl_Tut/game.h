#ifndef GAME_H
#define GAME_H

#include "spriterenderer.h"
#include "resourcemanager.h"
#include "gamelevel.h"
#include "texture.h"
#include "ballobject.h"
#include "particlegenerator.h"
#include "postprocessing.h"
#include "upgrade.h"
#include "textrenderer.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <irrKlang.h>

#include <random>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>


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
	float delta_time;
	float last_frame;
	float speed, player_speed;
	float shake_time;

	GLFWwindow* window;
	SpriteRenderer* renderer;
	//std::vector<GameLevel> levels;
	//unsigned int level;

	GameObject *player;
	Ball* ball;

	ParticleGenerator* particles;
	PostProcessor* postprocessor;

	std::vector<Upgrade> upgrades;

	irrklang::ISoundEngine* sound_engine;
	float sound_delay;

	TextRenderer* text_renderer;

	unsigned int lifes;
	float score;
	float play_time;
	float input_delay;
	bool gameover;

	bool check_collision(GameObject& ball, GameObject& object);
	Collision check_collision(Ball &ball, GameObject &object);
	Direction vector_direction(glm::vec2 target);

	void reset();
	void do_collisions();
	void render();
	void process_input(GLFWwindow* window, float dt);
	void spawn_upgrade(GameObject &object);
	void update_upgrades(float dt);
	void activate_upgrade(Upgrade& upgrade);
public:
	Game(unsigned int _width, unsigned int _height);
	~Game(); //Destructor
	void init();
	void update();
};

#endif