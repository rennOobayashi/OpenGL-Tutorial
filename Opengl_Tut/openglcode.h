#define STB_IMAGE_STATIC //전역 대신 정적 네임 스페이스를 사용하여 중복 정의 문제 방지
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#define X 1280
#define Y 720

class openglcode
{
private:
	unsigned int vbo, vao, veo;
	unsigned int shader_program;
	char info_log[512];

	const char* vertex_shader_source = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		"layout (location = 2) in vec2 aTexCoord;\n"
		"out vec3 ourColor;\n"
		"out vec2 texCoord;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos, 1.0);\n"
		"   ourColor = aColor;\n"
		"   texCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
		"}\0";

	const char* fragment_shader_source = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec3 ourColor;\n"
		"in vec2 texCoord;\n"
		"uniform sampler2D texture1;\n"
		"uniform sampler2D texture2;\n"
		"void main()\n"
		"{\n"
		"FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.05) * vec4(ourColor, 1.0);\n" //�ؽ��� �÷� ���ø��� ���� texture ���(�ؽ��� sampler, �ؽ��� �÷� �� ���ø�)
		"}\0";
	
	
	int width, height, color_ch;
	unsigned int texture1, texture2;
	unsigned char* data;
	void set_texture();

	void mk_shader();
	void draw_square();
	void process_input(GLFWwindow* window) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwWindowShouldClose(window);
		}
	}
public:
	void init();
	void set_n_run();
};