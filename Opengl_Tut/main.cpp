#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <iostream>
#define X 800
#define Y 600

class RunopenGL {
private:
	const char* vertex_shader_source = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	const char* fragment_shader_source = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";

public:
	void init() {
		glfwInit();
	}

	void set_n_run() {

		//opengl 3.3���� ���

		//������ ����
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		//���̳� ����
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		//glfw���� core profile�� ����Ѵٰ� �˷���
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		//��� ������ ������ ����

		GLFWwindow* window = glfwCreateWindow(X, Y, "Learn OpenGL", nullptr, nullptr);
		if (window == NULL)
		{
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
		//x, y ,z
		float vertices[] = {
			-0.5f,  -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f,  0.5f, 0.0f
		};
		unsigned int vbo, vao;
		unsigned int vertex_shader;
		unsigned int fragment_shader;
		unsigned int shader_program;
		int success;
		char info_log[512];

		//shader ����, vertex shader�� ����ϹǷ� GL_VERTEX_SHADER�� �Ķ����(�μ�)�� �Է�
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);

		//shader ��ü, �� ���� ���ڿ��� �Ǿ��ִ���, ���� �ҽ� �ڵ�
		glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
		//�������� �Ϸ�Ǿ����� Ȯ��
		glCompileShader(vertex_shader);

		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
		glCompileShader(fragment_shader);

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


		//���� ID ����, vertex buffer object�� ���� ������ GL_ARRAY_BUFFER
		glGenBuffers(1, &vbo);

		glGenVertexArrays(1, &vao);
	
		//vertex array ������Ʈ �ٿ��
		glBindVertexArray(vao);

		//Open�� ����ϱ� ���� vertex ����Ʈ ����
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		//OpenGL���� vertex �����͸� ��� �ؼ��ϴ��� �˷���
		//vertex �Ӽ�, vertex �Ӽ� ũ��, ������ Ÿ��, ������ ����ȭ ����, stride(vertex �Ӽ� ��Ʈ��  ���̰� ����), void*Ÿ���̹Ƿ� ����ȯ�ϰ� ��ġ �����Ͱ� �迭 ���� �κп� �����Ƿ� 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		while (!glfwWindowShouldClose(window)) {
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(shader_program);
			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, 3);

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
};

int main()
{
	RunopenGL run;

	run.init();
	run.set_n_run();

	return 0;
}