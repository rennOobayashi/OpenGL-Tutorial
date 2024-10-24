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
		//opengl 3.3버전 사용

		//메이져 버전
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		//마이너 버전
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		//glfw에게 core profile을 사용한다고 알려줌
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		//모든 윈도우 데이터 보유

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

			//컬러 버퍼(이미지 그리기 및 화면 출력) 교체
			glfwSwapBuffers(window);
			//이벤트 확인
			glfwPollEvents();
		}


		glfwTerminate();

		return;
	}
	/* Double buffer
	* 응용 프로그램이 single buffer로 이미지를 그렸을 때 깜빡거리는 현상
	* 이미지의 픽셀을 한번에 그리는 것이 아닌 왼쪽 위에서 오른쪽 아래순으로 그리기에 발생
	* 이를 해결하기 위해 앞 버퍼는 최종 출력 이미지를, 뒤 버퍼는 모든 렌더링 명령에서 그린 뒤 앞 버퍼와 교체
	*/

	void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		//0,0에서 시작, width,height에서 끝
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