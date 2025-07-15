#include "openglcode.h"

void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float last_x = X / 2.0f;
float last_y = Y / 2.0f;
bool first_mouse = true;

float lerp(float x, float y, float z) {
	return x + z * (y - x);
}

void openglcode::init() {
	glfwInit();

	//texture
	ResourceManager::load_texture("texture/floor.png", false, "background");
	ResourceManager::load_texture("texture/koronesuki.png", true, "face");
	ResourceManager::load_texture("texture/watashi.PNG", false, "block");
	ResourceManager::load_texture("texture/brick.jpg", false, "solid_block");

	//levels
	GameLevel level1;
	GameLevel level2;
	GameLevel level3;
	GameLevel level4;

}

void openglcode::run() {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 8);

	window = glfwCreateWindow(X, Y, "Learn OpenGL", nullptr, nullptr);
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

	glm::mat4 projection = glm::ortho(0.0f, (float)X, (float)Y, 0.0f, -1.0f, 1.0f);

	ResourceManager::load_shader("geofragver/vertex.vs", "geofragver/fragment.fs", nullptr, "sprite");
	ResourceManager::get_shader("sprite").use().set_int("img", 0);
	ResourceManager::get_shader("sprite").use().set_mat4("projection", projection);

	Shader shader = ResourceManager::get_shader("sprite");
	ResourceManager::load_texture("texture/watashi.PNG", true, "face");

	renderer = new SpriteRenderer(shader);

	Texture tex = ResourceManager::get_texture("face");

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		//clear depth value
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer->draw_sprite(tex, glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(1.0f, 1.0f, 1.0f));

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}