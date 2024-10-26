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

		//opengl 3.3버전 사용

		//메이져 버전
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		//마이너 버전
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		//glfw에게 core profile을 사용한다고 알려줌
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		//모든 윈도우 데이터 보유

		GLFWwindow* window = glfwCreateWindow(X, Y, "Learn OpenGL", nullptr, nullptr);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();

			return;
		}

		/* Double buffer
		* 응용 프로그램이 single buffer로 이미지를 그렸을 때 깜빡거리는 현상
		* 이미지의 픽셀을 한번에 그리는 것이 아닌 왼쪽 위에서 오른쪽 아래순으로 그리기에 발생
		* 이를 해결하기 위해 앞 버퍼는 최종 출력 이미지를, 뒤 버퍼는 모든 렌더링 명령에서 그린 뒤 앞 버퍼와 교체
		*/

		glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
			//0,0에서 시작, width,height에서 끝
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

		//shader 생성, vertex shader를 사용하므로 GL_VERTEX_SHADER를 파라미터(인수)로 입력
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);

		//shader 객체, 몇 개의 문자열로 되어있는지, 실제 소스 코드
		glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
		//컴파일이 완료되었는지 확인
		glCompileShader(vertex_shader);

		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
		glCompileShader(fragment_shader);

		//컴파일된 shader들을 연결하기 위한 program 객체, program 객체 ID 리턴
		shader_program = glCreateProgram();

		//컴파일한 shader 첨부
		glAttachShader(shader_program, vertex_shader);
		glAttachShader(shader_program, fragment_shader);
		//shader 연결
		glLinkProgram(shader_program);

		//shader 연결 성공 여부 확인
		glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

		if (!success) {
			glGetProgramInfoLog(shader_program, 512, NULL, info_log);
			std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << info_log << std::endl;
		}

		//연결 후 제거
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);


		//버퍼 ID 생성, vertex buffer object의 버퍼 유형은 GL_ARRAY_BUFFER
		glGenBuffers(1, &vbo);

		glGenVertexArrays(1, &vao);
	
		//vertex array 오브젝트 바운딩
		glBindVertexArray(vao);

		//Open이 사용하기 위해 vertex 리스트 복사
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		//OpenGL에게 vertex 데이터를 어떻게 해석하는지 알려줌
		//vertex 속성, vertex 속성 크기, 데이터 타입, 데이터 정규화 여부, stride(vertex 속성 세트들  사이간 공백), void*타입이므로 형변환하고 위치 데이터가 배열 시작 부분에 있으므로 0
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

			//컬러 버퍼(이미지 그리기 및 화면 출력) 교체
			glfwSwapBuffers(window);
			//이벤트 확인
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