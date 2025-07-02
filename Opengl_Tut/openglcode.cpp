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

	Shader shader("geofragver/vertex.vs", "geofragver/fragment.fs");
	spriterenderer renderer(shader);
	texture tex;

	glm::mat4 projection = glm::ortho(0.0f, (float)X, (float)Y, 0.0f, -1.0f, 1.0f);

	shader.use();
	shader.set_int("img", 0);
	shader.set_mat4("projection", projection);

	tex = load_texture("texture/watashi.PNG");


	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		//clear depth value
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.draw_sprite(tex, glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

texture openglcode::load_texture(char const* path) {
	texture tex;
	unsigned int id;
	int width, height, color_ch;
	GLenum format = GL_SRGB;

	data = stbi_load(path, &width, &height, &color_ch, 0);

	if (data)
	{
		if (color_ch == 1) {
			format = GL_RED;
		}
		else if (color_ch == 3) {
			format = GL_RGB;
		}
		else if (color_ch == 4) {
			format = GL_RGBA;
		}
		tex.internal_format = format;
		tex.image_format = format;

		tex.generate(width, height, data);

		stbi_set_flip_vertically_on_load(1);
	}
	else
	{
		std::cout << "Failed to load texture\n";
	}

	//mipmap 생성 후 이미지 메모리 반환
	stbi_image_free(data);

	return tex;
}