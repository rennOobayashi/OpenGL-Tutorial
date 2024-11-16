#include "openglcode.h"

void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
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
	light_pos = glm::vec3(0.0f, 5.0f, -0.0f);
	light_dir = glm::vec3(-0.2f, -1.0f, -0.3f);

	delta_time = 0.0f;
	last_frame = 0.0f;

	glfwInit();
}

void openglcode::set_n_run() {
	glm::vec3 cube_positions[] = {
		glm::vec3(0.0f, 0.0f, -2.0f),
		glm::vec3(8.0f, 5.0f, -15.0f),
		glm::vec3(-6.5f, -2.2f, -2.5f),
		glm::vec3(2.8f, -2.0f, -12.3f),
		glm::vec3(8.4f, -0.4f, -3.5f),
		glm::vec3(6.8f, 1.0f, -3.0f),
		glm::vec3(3.2f, 1.5f, -4.1f),
		glm::vec3(10.0f, 2.0f, 5.0f),
		glm::vec3(8.0f, 10.0f, 0.0f),
		glm::vec3(-7.5f, 4.5f, 1.0f),
		glm::vec3(-3.8f, -2.0f, -2.3f),
		glm::vec3(5.0f, 2.2f, 3.0f),
		glm::vec3(5.5f, 12.0f, 5.4f),
		glm::vec3(7.2f, -5.5f, 7.1f)
	};

	glm::vec3 point_lights_position[] = {
		glm::vec3(14.0f, 0.0f, 0.0f),
		glm::vec3(-10.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 5.0f, 0.0f),
		glm::vec3(0.0f, -5.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 10.0f),
		glm::vec3(0.0f, 0.0f, -10.0f)
	};

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	window = glfwCreateWindow(X, Y, "Learn OpenGL", nullptr, nullptr);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return;
	}

	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;

		return;
	}

	Shader light_shader("fragver/vertex.vs", "fragver/fragment.fs");
	Shader cube_shader("fragver/cube_vertex.vs", "fragver/cube_fragment.fs");

	draw_square();

	diff_tex = load_texture("texture/watashi.png");
	spec_tex = load_texture("texture/watashi_specular.png");

	light_shader.use();
	light_shader.set_int("material.diffuse", 0);
	light_shader.set_int("material.specular", 1);

	while (!glfwWindowShouldClose(window)) {
		float current_frame = (float)glfwGetTime();

		delta_time = current_frame - last_frame;
		last_frame = current_frame;


		process_input(window);

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		light_shader.use();

		for (int i = 0; i < 6; i++) {
			light_shader.set_vec3("point_lights[" + std::to_string(i) + "].position", point_lights_position[i]);
			light_shader.set_vec3("point_lights[" + std::to_string(i) + "].ambient", 0.5f, 0.05f, 0.5f);
			light_shader.set_vec3("point_lights[" + std::to_string(i) + "].diffuse", 0.3f, 0.3f, 0.3f);
			light_shader.set_vec3("point_lights[" + std::to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);
			light_shader.set_float("point_lights[" + std::to_string(i) + "].constant", 1.0f);
			light_shader.set_float("point_lights[" + std::to_string(i) + "].linear", 0.09f);
			light_shader.set_float("point_lights[" + std::to_string(i) + "].quadraitc", 0.032f);
		}

		light_shader.set_vec3("material.ambient", 1.0f, 0.5f, 0.31f);
		light_shader.set_vec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		light_shader.set_vec3("material.specular", 0.5f, 0.5f, 0.5f);
		light_shader.set_float("material.shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		light_shader.set_mat4("model", model);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diff_tex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, spec_tex);

		camera(light_shader);

		glBindVertexArray(vao);

		for (int i = 0; i < 14; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			float angle = 20.0f * i;

			model = glm::translate(model, cube_positions[i]);
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

			light_shader.set_mat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		
		cube_shader.use();
		camera(cube_shader);

		for (int i = 0; i < 6; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			float angle = 20.0f;

			model = glm::translate(model, point_lights_position[i]);
			model = glm::rotate(model, (float)glfwGetTime() * (glm::radians(angle) + 1), glm::vec3(1.0f, 0.3f, 0.5f));

			cube_shader.set_mat4("model", model);

			glBindVertexArray(cube_vao);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		

		//컬러 버퍼(이미지 그리기 및 화면 출력) 교체
		glfwSwapBuffers(window);
		//이벤트 확인
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &veo);
	glDeleteProgram(light_shader.id);

	glfwTerminate();

	return;
}

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void openglcode::draw_square() {
	//x, y ,z
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
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

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //offset 지정(3 * sizeof(float))
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	glGenVertexArrays(1, &cube_vao);
	glBindVertexArray(cube_vao);
	// VBO를 바인딩 바인딩하기만 하면 됩니다. 컨테이너의 VBO 데이터는 이미 정확한 데이터를 가지고 있습니다.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// vertex attribute를 설정합니다(램프를 위한 위치 데이터만).
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

unsigned int openglcode::load_texture(char const* path) {
	unsigned int texture;
	int width, height, color_ch;
	GLenum format;
	format = GL_RGB;

	glGenTextures(1, &texture);
	data = stbi_load(path, &width, &height, &color_ch, 0);

	if (data)
	{
		if (color_ch == 1) {
			format = GL_RED;
		}
		else if (color_ch == 3) {
			format = GL_RGB;
		}
		else if (color_ch == 4) {
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_set_flip_vertically_on_load(1);
	}
	else
	{
		std::cout << "Failed to load texture\n";
	}
	//mipmap 생성 후 이미지 메모리 반환

	stbi_image_free(data);

	return texture;
}

void openglcode::camera(Shader shader) {
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4  projection = glm::mat4(1.0f);
	float radius = 10.0f;
	float cam_x = sin((float)glfwGetTime()) * radius;
	float cam_z = cos((float)glfwGetTime()) * radius;

	//주변을 둘러볼수 있게 하는 3차원 방향 벡터 변환
	direction.y = sin(glm::radians(pitch));
	direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	direction.z = cos(glm::radians(pitch)) * cos(glm::radians(yaw));

	projection = glm::perspective(glm::radians(fov), (float)X / (float)Y, 0.1f, 100.0f);

	glfwSetScrollCallback(window, scroll_callback);

	view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);

	//행렬 데이터 shader에 보내기(uniform location, 보낼 행렬 수, 행과 열 바꿀지, 실제 행렬 데이터(OpenGL이 원하는 형태로 변환 후))
	shader.set_mat4("view", view);
	shader.set_mat4("projection", projection);

	//depth testing 수행 활성화
	glEnable(GL_DEPTH_TEST);
}

void openglcode::process_input(GLFWwindow* window) {
	float camera_speed = 2.5f * delta_time;

	//커서 안보이게 하고 창화면에 가둠
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	//대각선 이동이 가능하도록 하기 위해 전부 if로
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera_pos += camera_speed * camera_front;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera_pos -= camera_speed * camera_front;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
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
