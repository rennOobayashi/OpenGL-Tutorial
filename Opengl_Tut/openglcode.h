#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class openglcode
{
private:
	unsigned int vbo, vao, veo;
	unsigned int shader_program;
	char info_log[512];

	const char* vertex_shader_source = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n" //��ġ ������ attribute position 0�� ����
		"layout (location = 1) in vec3 aColor;\n" //�÷� ������ attribute position 1�� ����
		"out vec3 ourColor;\n" //�÷��� fragment shader�� ���
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos, 1.0);\n"
		"   ourColor = aColor;\n" //vertex data�κ��� ������ �÷� �Է��� ourColor�� ����
		"}\0";

	const char* fragment_shader_source = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec3 ourColor;\n" //Vertex Shader�� �Է� ���� �Է� ���� (�̸��� Ÿ���� ���ƾ� ��)
		"void main()\n"
		"{\n"
		"FragColor = vec4(ourColor, 1.0);\n"
		"}\0";

	//openglcode.cpp �̿ܿ��� ���� ���� ���Ѿ��� ������ ����
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

