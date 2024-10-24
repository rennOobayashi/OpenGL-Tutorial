#include <GLFW/glfw3.h>

#include <iostream>
#define X 800
#define Y 600

class RunopenGL {
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

		GLFWwindow* window = glfwCreateWindow(X, Y, "Learn OpenGL", NULL, NULL);

		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();

			return;
		}

		framebuffer_size_callback(window, X, Y);

		glfwMakeContextCurrent(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		while (!glfwWindowShouldClose(window)) {
			glClear(GL_COLOR_BUFFER_BIT);

			//�÷� ����(�̹��� �׸��� �� ȭ�� ���) ��ü
			glfwSwapBuffers(window);
			//�̺�Ʈ Ȯ��
			glfwPollEvents();
		}


		glfwTerminate();

		return;
	}
	/* Double buffer
	* ���� ���α׷��� single buffer�� �̹����� �׷��� �� �����Ÿ��� ����
	* �̹����� �ȼ��� �ѹ��� �׸��� ���� �ƴ� ���� ������ ������ �Ʒ������� �׸��⿡ �߻�
	* �̸� �ذ��ϱ� ���� �� ���۴� ���� ��� �̹�����, �� ���۴� ��� ������ ��ɿ��� �׸� �� �� ���ۿ� ��ü
	*/

	void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		//0,0���� ����, width,height���� ��
		glViewport(0, 0, width, height);
	}
};

int main()
{
	RunopenGL run;

	run.init();
	run.set_n_run();

	return 0;
}