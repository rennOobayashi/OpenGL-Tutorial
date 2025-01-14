#include "shader.h"
#include "model.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#define X 1280
#define Y 720

class openglcode
{
private:
	const unsigned int shadow_x = 512, shadow_y = 512;
	GLFWwindow* window;
	glm::vec3 camera_pos;
	glm::vec3 camera_up;
	glm::vec3 direction;
	glm::vec3 light_pos;
	glm::vec3 light_color;
	glm::vec3 light_dir;
	unsigned int vao, vbo, sao, sbo, fbo, depth_map, qao, qbo;
	char info_log[512];
	unsigned int texture1, texture2;
	unsigned int diff_tex, spec_tex;
	unsigned int cubemap_texture;
	unsigned char* data;
	float camera_speed;
	float delta_time;
	float last_frame;
	float outline_scale;

	void process_input(GLFWwindow* window);
	void draw_square();
	void draw_skybox();
	void framebuffer();
	void render_scene(const Shader &shader);
	unsigned int load_cubemap(std::vector<std::string> faces);
	unsigned int load_texture(char const* path);
public:
	void init();
	void set_n_run();
};