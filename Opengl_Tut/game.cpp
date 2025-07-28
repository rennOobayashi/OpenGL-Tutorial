#include "game.h"

void frame_buffer_size_callback(GLFWwindow* window, int width, int height);

const glm::vec2 player_size(120.0f, 30.0f);
const float player_velocity(500.0f);
const glm::vec2 initial_ball_velcocity(100.0f, -350.0f);
const float ball_radius = 12.5f;

Game::Game(unsigned int _width, unsigned int _height) 
	: states(GAME_ACTIVE), keys(), width(_width), height(_height) { }

Game::~Game() {
	delete renderer;
	delete player;
}

void Game::init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 8);

	window = glfwCreateWindow(width, height, "Learn OpenGL", nullptr, nullptr);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return;
	}

	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;

		return;
	}
	glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

	ResourceManager::load_shader("geofragver/vertex.vs", "geofragver/fragment.fs", nullptr, "sprite");
	ResourceManager::get_shader("sprite").use().set_int("img", 0);
	ResourceManager::get_shader("sprite").use().set_mat4("projection", projection);

	Shader shader = ResourceManager::get_shader("sprite");
	ResourceManager::load_texture("texture/watashi.PNG", true, "face");

	renderer = new SpriteRenderer(shader);

	//texture
	ResourceManager::load_texture("texture/floor.png", true, "background");
	ResourceManager::load_texture("texture/koronesuki.png", false, "ball");
	ResourceManager::load_texture("texture/watashi.PNG", true, "block");
	ResourceManager::load_texture("texture/brick.jpg", false, "solid_block");
	ResourceManager::load_texture("texture/paddle.png", true, "player");

	//levels
	GameLevel level1;
	GameLevel level2;
	GameLevel level3;
	GameLevel level4;

	level1.load("level/1.lvl", width, height / 2);
	level2.load("level/2.lvl", width, height / 2);
	level3.load("level/3.lvl", width, height / 2);
	level4.load("level/4.lvl", width, height / 2);

	levels.push_back(level1);
	levels.push_back(level2);
	levels.push_back(level3);
	levels.push_back(level4);
	level = 0;
	states = GAME_ACTIVE;

	glm::vec2 player_pos = glm::vec2(width / 2.0f - player_size.x / 2.0f, height - player_size.y - 30.0f);

	player = new GameObject(player_pos, player_size, ResourceManager::get_texture("player"));

	glm::vec2 ball_pos = player_pos + glm::vec2(
		player_size.x / 2.0f - ball_radius, -ball_radius * 2.5f);
	ball = new Ball(ball_pos, ball_radius, initial_ball_velcocity, ResourceManager::get_texture("ball"));
}

void Game::update() {
	
	while (!glfwWindowShouldClose(window)) {
		float current_frame = (float)glfwGetTime();

		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		//clear depth value
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		process_input(window, delta_time);
		ball->move(delta_time, width);
		do_collisions();

		if (states == GAME_ACTIVE) {
			render();
		}

		player->draw(*renderer);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}


void Game::render() {
	Texture tex = ResourceManager::get_texture("background");
	renderer->draw_sprite(tex, glm::vec2(0.0f, 0.0f), glm::vec2(width, height), 0.0f);

	levels[level].draw(*renderer);

	ball->draw(*renderer);
}

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void Game::process_input(GLFWwindow* window, float dt) {
	if (states == GAME_ACTIVE) {
		float velocity = player_velocity * dt;

		if (glfwGetKey(window, GLFW_KEY_A) || glfwGetKey(window, GLFW_KEY_LEFT)) {
			//Once you reach the left end, you won't move any further to the left.
			if (player->position.x >= 0.0f) {
				player->position.x -= velocity;

				if (ball->stuck) {
					ball->position.x -= velocity;
				}
			}
		}
		if (glfwGetKey(window, GLFW_KEY_D) || glfwGetKey(window, GLFW_KEY_RIGHT)) {
			//Once you reach the right end, you won't move any further to the right.
			if (player->position.x <= width - player->size.x) {
				player->position.x += velocity;

				if (ball->stuck) {
					ball->position.x += velocity;
				}
			}
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE)) {
			//Once you reach the right end, you won't move any further to the right.
			if (player->position.x <= width - player->size.x) {
				ball->stuck = false;
			}
		}
	}
}

bool Game::check_collision(GameObject &object1, GameObject &object2) {
	bool collision_x = object1.position.x + object1.size.x >= object2.position.x &&
		object2.position.x + object2.size.x >= object1.position.x;
	bool collision_y = object1.position.y + object1.size.y >= object2.position.y &&
		object2.position.y + object2.size.y >= object1.position.y;

	return collision_x && collision_y;
}

void Game::do_collisions() {
	for (GameObject& box : levels[level].bricks) {
		if (!box.destroyed) {
			if (check_collision(*ball, box)) {
				if (!box.is_solid) {
					box.destroyed = true;
				}
			}
		}
	}

	if (check_collision(*ball, *player)) {
		ball->velocity.y = -ball->velocity.y;
	}
}