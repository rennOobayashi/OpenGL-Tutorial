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
//		"out vec4 vertexColor;\n" //fragment shader�� ���� �÷� ��� ����
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//		"   vertexColor = vec4(0.5, 1.0, 0.0, 1.0);\n" //�� �ǰ��� ���� �ʷϻ�
		"}\0";

	const char* fragment_shader_source = "#version 330 core\n"
		"out vec4 FragColor;\n"
//		"in vec4 vertexColor;\n" //Vertex Shader�� �Է� ���� �Է� ���� (�̸��� Ÿ���� ���ƾ� ��)
		"uniform vec4 ourColor;\n" //������ ���� �� �����Ϸ��� �ڵ����� ���� ���� ���� ���� �� ����
		"void main()\n"
		"{\n"
//		"FragColor = vertexColor;\n"
		"FragColor = ourColor;\n"
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

