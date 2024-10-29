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
//		"out vec4 vertexColor;\n" //fragment shader를 위한 컬러 출력 지원
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//		"   vertexColor = vec4(0.5, 1.0, 0.0, 1.0);\n" //눈 건강에 좋은 초록색
		"}\0";

	const char* fragment_shader_source = "#version 330 core\n"
		"out vec4 FragColor;\n"
//		"in vec4 vertexColor;\n" //Vertex Shader로 입력 받은 입력 변수 (이름과 타입이 같아야 함)
		"uniform vec4 ourColor;\n" //사용되지 않을 시 컴파일러가 자동으로 삭제 시켜 오류 생길 수 있음
		"void main()\n"
		"{\n"
//		"FragColor = vertexColor;\n"
		"FragColor = ourColor;\n"
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

