#include "openglcode.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#define X 800
#define Y 600

void openglcode::init() {
	glfwInit();
}

void openglcode::set_n_run() {
	//opengl 3.3���� ���

	//������ ����
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//���̳� ����
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfw���� core profile�� ����Ѵٰ� �˷���
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//��� ������ ������ ����

	GLFWwindow* window = glfwCreateWindow(X, Y, "Learn OpenGL", nullptr, nullptr);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return;
	}

	/* Double buffer
	* ���� ���α׷��� single buffer�� �̹����� �׷��� �� �����Ÿ��� ����
	* �̹����� �ȼ��� �ѹ��� �׸��� ���� �ƴ� ���� ������ ������ �Ʒ������� �׸��⿡ �߻�
	* �̸� �ذ��ϱ� ���� �� ���۴� ���� ��� �̹�����, �� ���۴� ��� ������ ��ɿ��� �׸� �� �� ���ۿ� ��ü
	*/

	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		//0,0���� ����, width,height���� ��
		glViewport(0, 0, width, height);
		});

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;

		return;
	}

	mk_shader();
	draw_square();


	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader_program);
		glBindVertexArray(vao);
		//glDrawArrays(GL_TRIANGLES, 0, 4);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//�÷� ����(�̹��� �׸��� �� ȭ�� ���) ��ü
		glfwSwapBuffers(window);
		//�̺�Ʈ Ȯ��
		glfwPollEvents();
	}
	glDeleteBuffers(0, &vbo);
	glDeleteProgram(shader_program);

	glfwTerminate();

	return;
}

void openglcode::mk_shader() {
	unsigned int vertex_shader;
	unsigned int fragment_shader;
	int success;

	//shader ����, vertex shader�� ����ϹǷ� GL_VERTEX_SHADER�� �Ķ����(�μ�)�� �Է�
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);

	//shader ��ü, �� ���� ���ڿ��� �Ǿ��ִ���, ���� �ҽ� �ڵ�
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	//�������� �Ϸ�Ǿ����� Ȯ��
	glCompileShader(vertex_shader);

	glGetProgramiv(vertex_shader, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(vertex_shader, 512, NULL, info_log);
		std::cout << "ERROR::VERTEX::SHADER::COMPILATION_FAILED\n" << info_log << std::endl;
	}

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	glGetProgramiv(fragment_shader, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(fragment_shader, 512, NULL, info_log);
		std::cout << "ERROR::FRAGMENT::SHADER::COMPILATION_FAILED\n" << info_log << std::endl;
	}

	//�����ϵ� shader���� �����ϱ� ���� program ��ü, program ��ü ID ����
	shader_program = glCreateProgram();

	//�������� shader ÷��
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	//shader ����
	glLinkProgram(shader_program);

	//shader ���� ���� ���� Ȯ��
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << info_log << std::endl;
	}

	//���� �� ����
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

void openglcode::draw_square() {
	//x, y ,z
	float vertices[] = {
		-0.4f,  0.5f, 0.0f, //0
		-0.4f, -0.5f, 0.0f, //1
		0.4f, 0.5f, 0.0f,   //2
		0.4f,  -0.5f, 0.0f  //3
	};
	//�ﰢ�� �� ��ġ
	unsigned int indices[] = {
		0, 1, 2,
		1, 2, 3
	};

	//���� ID ����, vertex buffer object�� ���� ������ GL_ARRAY_BUFFER
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &veo);

	glGenVertexArrays(1, &vao);

	//vertex array ������Ʈ �ٿ��
	glBindVertexArray(vao);

	//Open�� ����ϱ� ���� vertex ����Ʈ ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, veo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//OpenGL���� vertex �����͸� ��� �ؼ��ϴ��� �˷���
	//vertex �Ӽ�, vertex �Ӽ� ũ��, ������ Ÿ��, ������ ����ȭ ����, stride(vertex �Ӽ� ��Ʈ��  ���̰� ����), void*Ÿ���̹Ƿ� ����ȯ�ϰ� ��ġ �����Ͱ� �迭 ���� �κп� �����Ƿ� 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}