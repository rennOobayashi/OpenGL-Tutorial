#include "openglcode.h"

void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);

glm::vec3 camera_front;
float yaw = -90.0f;
float pitch = 0.0f;
float last_x = X / 2;
float last_y = Y / 2;
bool first_mouse = true;
float sensivity = 0.05f;
float fov = 45.0f;;

void openglcode::init() {
	camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
	camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
	camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

	delta_time = 0.0f;
	last_frame = 0.0f;

	glfwInit();
}

void openglcode::set_n_run() {
	glm::vec3 cube_positions[] = {
		glm::vec3(0.0f, 0.0f, -2.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.8f, 1.0f, -3.0f),
		glm::vec3(3.2f, 1.5f, -4.1f),
	};
	//opengl 3.3버전 사용

	//메이져 버전
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//마이너 버전
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfw에게 core profile(가장 기본적이고 필수적인 정보)을 사용한다고 알려줌
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//모든 윈도우 데이터 보유

	window = glfwCreateWindow(X, Y, "Learn OpenGL", nullptr, nullptr);
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
		float current_frame = glfwGetTime();

		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		process_input(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glUseProgram(shader_program);

		camera();

		glBindVertexArray(vao);

		for (unsigned int i = 0; i < 7; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			float angle = 20.0f * i;

			model = glm::translate(model, cube_positions[i]);
			model = glm::rotate(model, (float)glfwGetTime() * (glm::radians(angle) + 1), glm::vec3(1.0f, 0.3f, 0.5f));
			
			glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, &model[0][0]);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

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
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//컬러 attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //offset 지정(3 * sizeof(float))
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //offset 지정(3 * sizeof(float))
	glEnableVertexAttribArray(2);

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

void openglcode::camera() {
	unsigned int view_loc = glGetUniformLocation(shader_program, "view");
	unsigned int projection_loc = glGetUniformLocation(shader_program, "projection");
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4  projection = glm::mat4(1.0f);
	float radius = 10.0f;
	float cam_x = sin(glfwGetTime()) * radius;
	float cam_z = cos(glfwGetTime()) * radius;

	//주변을 둘러볼수 있게 하는 3차원 방향 벡터 변환
	direction.y = sin(glm::radians(pitch));
	direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	direction.z = cos(glm::radians(pitch)) * cos(glm::radians(yaw));

	projection = glm::perspective(glm::radians(fov), (float)X / (float)Y, 0.1f, 100.0f);

	glfwSetScrollCallback(window, scroll_callback);

	view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);

	//행렬 데이터 shader에 보내기(uniform location, 보낼 행렬 수, 행과 열 바꿀지, 실제 행렬 데이터(OpenGL이 원하는 형태로 변환 후))
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, &projection[0][0]);

	//depth testing 수행 활성화
	glEnable(GL_DEPTH_TEST);
}

void openglcode::process_input(GLFWwindow* window) {
	float camera_speed = 2.5f * delta_time;

	camera_speed = 0.05f;

	//커서 안보이게 하고 창화면에 가둠
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera_pos += camera_speed * camera_front;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera_pos -= camera_speed * camera_front;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		std::cout << "EXIT\n";
		glfwSetWindowShouldClose(window, true);
	}
}

void mouse_callback(GLFWwindow* window, double x_pos, double y_pos) {
	if (first_mouse) {
		last_x = x_pos;
		last_y = y_pos;

		first_mouse = false;
	}

	glm::vec3 front;
	float x_offset = x_pos - last_x;
	float y_offset = last_y - y_pos;

	last_x = x_pos;
	last_y = y_pos;

	x_offset *= sensivity;
	y_offset *= sensivity;

	yaw += x_offset;
	pitch += y_offset;

	//y축 카메라 시점 제한
	if (pitch > 89.0f) {
		pitch = 89.0f;
	} else if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camera_front = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset) {
	if (fov >= 1.0f && fov <= 45.0f) {
		fov -= y_offset;
	} else if (fov <= 1.0f) {
		fov = 1.0f;
	}
	else if (fov >= 45.0f) {
		fov = 45.0f;
	}
}