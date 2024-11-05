#include "openglcode.h"

void openglcode::init() {
	glfwInit();
}

void openglcode::set_n_run() {
	//opengl 3.3버전 사용

	//메이져 버전
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//마이너 버전
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfw에게 core profile(가장 기본적이고 필수적인 정보)을 사용한다고 알려줌
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//모든 윈도우 데이터 보유

	GLFWwindow* window = glfwCreateWindow(X, Y, "Learn OpenGL", nullptr, nullptr);
	if (window == NULL) {
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

	mk_shader();
	draw_square();
	set_texture();

	while (!glfwWindowShouldClose(window)) {

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		coordinate();

		glUseProgram(shader_program);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		//컬러 버퍼(이미지 그리기 및 화면 출력) 교체
		glfwSwapBuffers(window);
		//이벤트 확인
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &veo);
	glDeleteProgram(shader_program);

	glfwTerminate();

	return;
}

void openglcode::mk_shader() {
	unsigned int vertex_shader;
	unsigned int fragment_shader;
	int success;

	//shader 생성, vertex shader를 사용하므로 GL_VERTEX_SHADER를 파라미터(인수)로 입력
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);



	//shader 객체, 몇 개의 문자열로 되어있는지, 실제 소스 코드
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	//컴파일이 완료되었는지 확인
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
}

void openglcode::draw_square() {
	//x, y ,z
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	//삼각형 점 위치
	unsigned int indices[] = {
		0, 1, 3,
		1, 3, 2
	};

	//버퍼 ID 생성, vertex buffer object의 버퍼 유형은 GL_ARRAY_BUFFER
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &veo);

	glGenVertexArrays(1, &vao);

	//vertex array 오브젝트 바운딩
	glBindVertexArray(vao);

	//Open이 사용하기 위해 vertex 리스트 복사
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, veo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//OpenGL에게 vertex 데이터를 어떻게 해석하는지 알려줌
	//vertex 속성, vertex 속성 크기, 데이터 타입, 데이터 정규화 여부, stride(vertex 속성 세트들 사이간 공백), void*타입이므로 형변환하고 위치 데이터가 배열 시작 부분에 있으므로 0
	//위치 attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//컬러 attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); //offset 지정(3 * sizeof(float))
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void openglcode::set_texture() {

	//텍스쳐 갯수, 텍스쳐 배열 저장
	glGenTextures(1, &texture1);
	//바인딩
	glBindTexture(GL_TEXTURE_2D, texture1);

	//텍스쳐 타겟 지정(2D나 3D등  지정, 설정할 옵션(WRAP)과 적용할 축, 텍스쳐 모드 (위치가 1.0보다 클시 어떻게 반복시킬지, GL_CLAMP_TO_BORDER일 시 glTexParmeterfv를 사용하여 태두리색 또한 설정))
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//텍스쳐 필터링 방법 지정
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //or GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //or GL_LINEAR

	data = stbi_load("watashi.png", &width, &height, &color_ch, 0);

	if (data) 
	{
		//텍스쳐 생성(텍스쳐 타겟, 텍스쳐 mipmap 레벨 수동 지정 여부 (아닐시 0), OpenGL에 우리가 저장하고 싶은 텍스쳐 포멧 지정, 텍스쳐 너비, 높이, 그냥 0, 원본 이미지 포멧과 데이터 타입, 실제 이미지 데이터)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture\n";
	}
	//mipmap 생성 후 이미지 메모리 반환

	stbi_image_free(data);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(1);
	data = stbi_load("sakura.png", &width, &height, &color_ch, 0);
	//std::cout << width << ", " << height << std::endl;
	if (data) 
	{
		//텍스쳐 생성(텍스쳐 타겟, 텍스쳐 mipmap 레벨 수동 지정 여부 (아닐시 0), OpenGL에 우리가 저장하고 싶은 텍스쳐 포멧 지정, 텍스쳐 너비, 높이, 그냥 0, 원본 이미지 포멧과 데이터 타입, 실제 이미지 데이터)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
	{
		std::cout << "Failed to load texture\n";
	}
	//mipmap 생성 후 이미지 메모리 반환
	stbi_image_free(data);

	glUseProgram(shader_program);
	glUniform1i(glGetUniformLocation(shader_program, "texture1"), 0);
	glUniform1i(glGetUniformLocation(shader_program, "texture2"), 1);
}

void openglcode::transform() {
	unsigned int transform_loc = glGetUniformLocation(shader_program, "transform");
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f); //x, y, z, t
	glm::mat4 trans = glm::mat4(1.0f); //4x4단위 행렬

	//translate로 변환 행렬 생성 후 trans에 반환
	trans = glm::translate(trans, glm::vec3(0.75f, -0.75f, 0.0f));
	trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));

	//행렬 데이터 shader에 보내기(uniform location, 보낼 행렬 수, 행과 열 바꿀지, 실제 행렬 데이터(OpenGL이 원하는 형태로 변환 후))
	glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(trans));
}

void openglcode::coordinate() {
	unsigned int model_loc = glGetUniformLocation(shader_program, "model");
	unsigned int view_loc = glGetUniformLocation(shader_program, "view");
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4  projection = glm::mat4(1.0f);

	model = glm::rotate(model, (float)glfwGetTime() * glm::radians(55.0f), glm::vec3(0.5f, 1.0f, 0.0f));
	//움직이고 싶은 방향의 반대로
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));
	projection = glm::perspective(glm::radians(45.0f), (float)X / (float)Y, 0.1f, 100.0f);

	//행렬 데이터 shader에 보내기(uniform location, 보낼 행렬 수, 행과 열 바꿀지, 실제 행렬 데이터(OpenGL이 원하는 형태로 변환 후))
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, &projection[0][0]);

}