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
		"layout (location = 0) in vec3 aPos;\n" //위치 변수는 attribute position 0를 가짐
		"layout (location = 1) in vec3 aColor;\n" //컬러 변수는 attribute position 1를 가짐
		"out vec3 ourColor;\n" //컬러를 fragment shader로 출력
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos, 1.0);\n"
		"   ourColor = aColor;\n" //vertex data로부터 가져와 컬러 입력을 ourColor에 설정
		"}\0";

	const char* fragment_shader_source = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec3 ourColor;\n" //Vertex Shader로 입력 받은 입력 변수 (이름과 타입이 같아야 함)
		"void main()\n"
		"{\n"
		"FragColor = vec4(ourColor, 1.0);\n"
		"}\0";

	//openglcode.cpp 이외에는 따로 실행 시켜야할 이유가 없음
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

