#define STB_IMAGE_STATIC //전역 대신 정적 네임 스페이스를 사용하여 중복 정의 문제 방지
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#define X 1280
#define Y 720

class openglcode
{
private:
	GLFWwindow* window;
	glm::vec3 camera_pos;
	glm::vec3 camera_up;
	glm::vec3 direction;
	glm::vec3 light_pos;
	glm::vec3 light_color;
	glm::vec3 light_dir;
	unsigned int vbo, vao, veo, cube_vao;
	char info_log[512];
	unsigned int texture1, texture2;
	unsigned int diff_tex, spec_tex;
	unsigned char* data;
	float camera_speed;
	float delta_time;
	float last_frame;

	void draw_square();
	unsigned int load_texture(char const* path);
	void process_input(GLFWwindow* window);
	void camera(Shader shader);
public:
	void init();
	void set_n_run();
};