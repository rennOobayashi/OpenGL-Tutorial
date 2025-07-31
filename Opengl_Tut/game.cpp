#include "game.h"

void frame_buffer_size_callback(GLFWwindow* window, int width, int height);

const glm::vec2 player_size(120.0f, 30.0f);
const float player_velocity(500.0f);
const glm::vec2 initial_ball_velcocity(100.0f, -350.0f);
const float ball_radius = 12.5f;

Game::Game(unsigned int _width, unsigned int _height) 
	: states(GAME_ACTIVE), width(_width), height(_height) { }

Game::~Game() {
	delete renderer;
	delete player;
	delete ball;
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
	speed = 1.0f;
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
		ball->move(delta_time, width, speed);
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

Collision Game::check_collision(Ball& ball, GameObject& object) {
	//ball's center
	glm::vec2 center(ball.position + ball.radius);
	//calculate aabb info
	glm::vec2 aabb_half_extents(object.size.x / 2.0f, object.size.y / 2.0f);
	glm::vec2 aabb_center(object.position.x + aabb_half_extents.x, object.position.y + aabb_half_extents.y);
	//get difference between ball's center and aabb's center
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	//get closest point on aabb to ball's center
	glm::vec2 closest = aabb_center + clamped;
	//search closest point and check if lengh < radius
	difference = closest - center;

	//<= is used when exactly touching
	//but lengh < radius is used as < because it is in the last step of check collision
	if (glm::length(difference) < ball.radius) {
		return std::make_tuple(true, vector_direction(difference), difference);
	}
	else {
		return std::make_tuple(false, UP, glm::vec2(0.0f));
	}

}

void Game::do_collisions() {
	for (GameObject& box : levels[level].bricks) {
		if (!box.destroyed) {
			Collision collision = check_collision(*ball, box);
			//if collided
			if (std::get<0>(collision)) {
				if (!box.is_solid) {
					box.destroyed = true;
					speed += 0.025f;
				}

				Direction dir = std::get<1>(collision);

				glm::vec2 diff_vec = std::get<2>(collision);

				if (dir == LEFT || dir == RIGHT) {
					ball->velocity.x = -ball->velocity.x;

					//relocate
					float penetration = ball->radius - std::abs(diff_vec.x);

					if (dir == LEFT) {
						ball->position.x += penetration; //move ball right
					}
					else {
						ball->position.x -= penetration; //move ball left
					}
				}
				else {
					ball->velocity.y = -ball->velocity.y;

					float penetration = ball->radius - std::abs(diff_vec.x);

					if (dir == UP) {
						ball->position.y -= penetration;  //move ball down
					}
					else {
						ball->position.y += penetration;  //move ball up
					}
				}
			}
		}
	}
}

Direction Game::vector_direction(glm::vec2 target) {
	glm::vec2 compass[] = {
		glm::vec2( 0.0f,  1.0f), //up
		glm::vec2( 0.0f, -1.0f), //down
		glm::vec2(-1.0f,  0.0f), //left
		glm::vec2( 1.0f,  0.0f)  //right
	};
	float max = 0.0f;
	unsigned int best_match = -1;

	//brick's kita, minami, nishi, higashi
	for (unsigned int i = 0; i < 4; i++) {
		float dot_product = glm::dot(glm::normalize(target), compass[i]);

		if (dot_product > max) {
			max = dot_product;
			best_match = i;
		}
	}

	return (Direction)best_match;
}