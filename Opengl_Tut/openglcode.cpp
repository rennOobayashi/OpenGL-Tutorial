#include "openglcode.h"

void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);
//void gl_debug_output(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float last_x = X / 2.0f;
float last_y = Y / 2.0f;
bool first_mouse = true;

float lerp(float x, float y, float z) {
	return x + z * (y - x);
}

void openglcode::init() {
	light_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	light_dir = glm::vec3(-0.2f, -1.0f, -0.3f);

	delta_time = 0.0f;
	last_frame = 0.0f;
	exposure = 2.0;

	glfwInit();

	if (FT_Init_FreeType(&ft)) {
		throw 1;
	}

	if (FT_New_Face(ft, "fonts/arial.ttf", 0, &fface)) {
		throw 2;
	}

	//Dynamically allocate font size from 0 ~ 48 pixels
	FT_Set_Pixel_Sizes(fface, 0, 48);

	//test text load
	if (FT_Load_Char(fface, 'X', FT_LOAD_RENDER)) {
		throw 2;
	}

	load_font();
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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 8);
	//It is recommended to remove this request hint when releasing,
	//as it may slow down the speed compared to the context.
	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


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
	
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

	/*
	if (flags && GL_CONTEXT_FLAG_DEBUG_BIT) {
		glEnable(GL_DEBUG_OUTPUT);
		//Tell OpenGL to print debug output
		//and call a callback function when an error occurs
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(gl_debug_output, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		//Only high severity error messages print
		//glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE);
		//Custom error message
		//glDebugMessageInsert(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, 0, GL_DEBUG_SEVERITY_HIGH, -1, "Error message here");
	}*/

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//appropriately filters cubemap faces
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader shader("geofragver/vertex.vs", "geofragver/fragment.fs");
	Shader hdr_shader("geofragver/cubemap_vertex.vs", "geofragver/cubemap_fragment.fs");
	Shader irradiance_shader("geofragver/cubemap_vertex.vs", "geofragver/irradiance_fragment.fs");
	Shader prefilter_shader("geofragver/cubemap_vertex.vs", "geofragver/hammersley_fragment.fs");
	Shader brdf_shader("geofragver/brdf_vertex.vs", "geofragver/brdf_fragment.fs");
	Shader background_shader("geofragver/background_vertex.vs", "geofragver/background_fragment.fs");
	//Model rock_model("model/rock/rock.obj");
	
	draw_square();
	draw_sphere();

	glm::vec3 light_positions[] = {
		glm::vec3(-10.0f,  10.0f, 10.0f),
		glm::vec3(10.0f,  10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3(10.0f, -10.0f, 10.0f),
	};
	glm::vec3 light_colors[] = {
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f)
	};

	int nr_rows = 1;
	int nr_columns = 9;
	float spacing = 2.1f;

	albedo_tex = load_texture("texture/pbr/albedo.png");
	nor_tex = load_texture("texture/pbr/normal.png");
	metal_tex = load_texture("texture/pbr/metal.png");
	rough_tex = load_texture("texture/pbr/rough.png");
	ao_tex = load_texture("texture/pbr/ao.png");

	shader.use();
	shader.set_int("irradiance_map", 0);
	shader.set_int("prefilter_map", 1);
	shader.set_int("brdf_lut", 2);
	shader.set_int("albedo_map", 3);
	shader.set_int("normal_map", 4);
	shader.set_int("metallic_map", 5);
	shader.set_int("roughness_map", 6);
	shader.set_int("ao_map", 7);

	background_shader.use();
	shader.set_int("envronment_map", 0);


	draw_skybox(hdr_shader, irradiance_shader, prefilter_shader, brdf_shader);
	
	int scr_width, scr_height;	
	glfwGetFramebufferSize(window, &scr_width, &scr_height);
	glViewport(0, 0, scr_width, scr_height);

	while (!glfwWindowShouldClose(window)) {
		float current_frame = (float)glfwGetTime();

		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		process_input(window);

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		//clear depth value
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//std::cout << camera.fov << std::endl;
		glm::mat4 view = camera.get_view_matrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)X / (float)Y, 0.1f, 100.0f);
		glm::mat4 model = glm::mat4(1.0f);
		shader.use();
		shader.set_mat4("view", view);
		shader.set_mat4("projection", projection);
		shader.set_vec3("cam_pos", camera.position);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, irradiance_map);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, prefilter_map);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, brdf_texture);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, albedo_tex);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, nor_tex);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, metal_tex);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, rough_tex);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, ao_tex);
		//rock_model.draw(shader);

		for (int i = 0; i < nr_rows; i++) {
			shader.set_float("metallic", (float)i / (float)nr_rows);
			for (int j = 0; j < nr_columns; j++) {
				shader.set_float("roughness", glm::clamp((float)j / (float)nr_columns, 0.05f, 1.0f));
				
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(
					(j - (nr_columns / 2)) * spacing, //x
					(i - (nr_rows / 2)) * spacing,    //y
					0.0f));							  //z
				shader.set_mat4("model", model);
				shader.set_mat3("normal_matrix", glm::transpose(glm::inverse(glm::mat3(model))));

				glBindVertexArray(sphere_vao);
				glDrawElements(GL_TRIANGLE_STRIP, index_cnt, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
		}

		for (unsigned int i = 0; i < sizeof(light_positions) / sizeof(light_positions[0]); i++) {
			glm::vec3 new_pos = light_positions[i] + glm::vec3(sin(glfwGetTime() * 5.0f) * 5.0f, 0.0f, 0.0f);
			shader.set_vec3("light_positions[" + std::to_string(i) + "]", new_pos);
			shader.set_vec3("light_colors[" + std::to_string(i) + "]", light_colors[i]);

			model = glm::mat4(1.0f);
			model = glm::translate(model, new_pos);
			model = glm::scale(model, glm::vec3(0.5f));
			shader.set_mat4("model", model);
			shader.set_mat3("normal_matrix", glm::transpose(glm::inverse(glm::mat3(model))));

			glBindVertexArray(sphere_vao);
			glDrawElements(GL_TRIANGLE_STRIP, index_cnt, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		background_shader.use();
		background_shader.set_mat4("view", view);
		background_shader.set_mat4("projection", projection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, env_cubemap);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		//brdf map print to screen
		//brdf_shader.use();
		//glBindVertexArray(qao);
		//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		//glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &sphere_vao);
	glDeleteBuffers(1, &eao);
	glDeleteBuffers(1, &ebo);
	glDeleteFramebuffers(1, &fbo);
	glDeleteFramebuffers(1, &capture_fbo);
	glDeleteRenderbuffers(1, color_buffer);
	glDeleteRenderbuffers(1, &capture_rbo);
	glDeleteProgram(shader.id);
	glDeleteProgram(hdr_shader.id);
	glDeleteProgram(background_shader.id);
	glDeleteProgram(irradiance_shader.id);
	glDeleteProgram(brdf_shader.id);
	glDeleteProgram(prefilter_shader.id);

	glfwTerminate();

	return;
}

unsigned int openglcode::load_cubemap(std::vector<std::string> faces) {
	unsigned int texture_id;
	int width, height, color_ch;
	GLenum format = GL_RGB;

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &color_ch, 0);

		if (data) {
			if (color_ch == 1) {
				format = GL_RED;
			}
			else if (color_ch == 3) {
				format = GL_RGB;
			}
			else if (color_ch == 4) {
				format = GL_RGBA;
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		}
		else {
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
		}

		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return texture_id;
}

void openglcode::draw_skybox(Shader hdr_shader, Shader irradiance_shader, Shader prefilter_shader, Shader brdf_shader) {
	glm::mat4 capture_views[] = {
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	glGenFramebuffers(1, &capture_fbo);
	glGenRenderbuffers(1, &capture_rbo);

	glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, capture_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, capture_rbo);

	stbi_set_flip_vertically_on_load(true);
	int width, height, color_ch;
	float* data = stbi_loadf("texture/fhd_skybox.hdr", &width, &height, &color_ch, 0);

	if (data) {
		glGenTextures(1, &hdr_texture);
		glBindTexture(GL_TEXTURE_2D, hdr_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load HDR image." << std::endl;
	}

	glGenTextures(1, &env_cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, env_cubemap);
	//gl_check_error();

	for (unsigned int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glm::mat4 capture_projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	hdr_shader.use();
	hdr_shader.set_int("equirectangular_map", 0);
	hdr_shader.set_mat4("projection", capture_projection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdr_texture);

	glViewport(0, 0, 512, 512);
	glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo);

	for (unsigned int i = 0; i < 6; i++) {
		hdr_shader.set_mat4("view", capture_views[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, env_cubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, env_cubemap);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glGenTextures(1, &irradiance_map);
	glBindTexture(GL_TEXTURE_CUBE_MAP, irradiance_map);

	for (unsigned int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, capture_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

	irradiance_shader.use();
	irradiance_shader.set_int("envronment_map", 0);
	irradiance_shader.set_mat4("projection", capture_projection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, env_cubemap);

	glViewport(0, 0, 32, 32);
	glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo);

	for (unsigned int i = 0; i < 6; i++) {
		irradiance_shader.set_mat4("view", capture_views[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradiance_map, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glGenTextures(1, &prefilter_map);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilter_map);

	for (unsigned int i = 0; i < 6; i++) {
		//it is sufficient for most reflections, but if you have a lot of soft materials, you may want to increase it
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	prefilter_shader.use();
	prefilter_shader.set_int("envronment_map", 0);
	prefilter_shader.set_mat4("projection", capture_projection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, env_cubemap);

	glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo);
	unsigned int max_mip_levels = 5;

	for (unsigned int mip = 0; mip < max_mip_levels; mip++) {
		unsigned int mip_width = 128 * std::pow(0.5, mip);
		unsigned int mip_height = 128 * std::pow(0.5, mip);

		glBindRenderbuffer(GL_RENDERBUFFER, capture_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mip_width, mip_height);
		glViewport(0, 0, mip_width, mip_height);

		float roughness = (float)mip / (float)(max_mip_levels - 1);
		prefilter_shader.set_float("roughness", roughness);

		for (unsigned int i = 0; i < 6; i++) {
			prefilter_shader.set_mat4("view", capture_views[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilter_map, mip);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenTextures(1, &brdf_texture);
	glBindTexture(GL_TEXTURE_2D, brdf_texture);
	//use 16 bit precision floating format
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F,  512, 512, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, capture_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdf_texture, 0);
	
	glViewport(0, 0, 512, 512);
	brdf_shader.use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glBindVertexArray(qao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void openglcode::draw_square() {
	float vertices[] = {
		//back
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
		//front
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
		//left
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		//right
		 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		 //bottom
		 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
		  1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		 -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
		 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
		 //top
		 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
		  1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		  1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
		 -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f
	};

	//버퍼 ID 생성, vertex buffer object의 버퍼 유형은 GL_ARRAY_BUFFER
	glGenVertexArrays(1, &vao);

	glGenBuffers(1, &vbo);

	//vertex array 오브젝트 바운딩
	glBindVertexArray(vao);

	//Open이 사용하기 위해 vertex 리스트 복사
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	//OpenGL에게 vertex 데이터를 어떻게 해석하는지 알려줌
	//vertex 속성, vertex 속성 크기, 데이터 타입, 데이터 정규화 여부, stride(vertex 속성 세트들 사이간 공백), void*타입이므로 형변환하고 위치 데이터가 배열 시작 부분에 있으므로 0
	//위치 attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	glBindVertexArray(0);


	float quad_vertices[] = {
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &qao);
	glGenBuffers(1, &qbo);
	glBindVertexArray(qao);
	glBindBuffer(GL_ARRAY_BUFFER, qbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
}

void openglcode::draw_sphere() {
	std::vector<glm::vec3> position;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uv;
	std::vector<unsigned int> indices;
	std::vector<float> data;
	const unsigned int x_segments = 64;
	const unsigned int y_segments = 64;
	const float pi = 3.14159265359f;
	bool odd_rows = false;

	glGenVertexArrays(1, &sphere_vao);

	glGenBuffers(1, &eao);
	glGenBuffers(1, &ebo);

	for (unsigned int x = 0; x <= x_segments; x++) {
		for (unsigned int y = 0; y <= y_segments; y++) {
			float _x_segment = (float)x / (float)x_segments;
			float _y_segment = (float)y / (float)y_segments;
			float x_pos = std::cos(_x_segment * 2.0f * pi) * std::sin(_y_segment * pi);
			float y_pos = std::cos(_y_segment * pi);
			float z_pos = std::sin(_x_segment * 2.0f * pi) * std::sin(_y_segment * pi);

			position.push_back(glm::vec3(x_pos, y_pos, z_pos));
			uv.push_back(glm::vec2(_x_segment, _y_segment));
			normals.push_back(glm::vec3(x_pos, y_pos, z_pos));
		}
	}

	//std::cout << "1 clear\n";

	for (unsigned int y = 0; y < y_segments; y++) {
		if (!odd_rows) {
			for (unsigned int x = 0; x <= x_segments; x++) {
				indices.push_back(y * (x_segments + 1) + x);
				indices.push_back((y + 1) * (x_segments + 1) + x);
			}
		}
		else {
			for (int x = x_segments; x >= 0; x--) {
				indices.push_back((y + 1) * (x_segments + 1) + x);
				indices.push_back(y * (x_segments + 1) + x);
			}
		}
		odd_rows = !odd_rows;
	}
	//std::cout << "2 clear\n";

	index_cnt = indices.size();

	for (unsigned int i = 0; i < position.size(); i++) {
		data.push_back(position[i].x);
		data.push_back(position[i].y);
		data.push_back(position[i].z);

		if (normals.size() > 0) {
			data.push_back(normals[i].x);
			data.push_back(normals[i].y);
			data.push_back(normals[i].z);
		}

		if (uv.size() > 0) {
			data.push_back(uv[i].x);
			data.push_back(uv[i].y);
		}
	}
	//std::cout << "3 clear\n";

	unsigned int stride = (3 + 2 + 3) * sizeof(float);
	glBindVertexArray(sphere_vao);
	glBindBuffer(GL_ARRAY_BUFFER, eao);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
}

unsigned int openglcode::load_texture(char const* path) {
	unsigned int texture;
	int width, height, color_ch;
	GLenum format = GL_SRGB;

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
/*
void openglcode::multi_sample() {
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenTextures(1, &texcolorbuffer_multisampled);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texcolorbuffer_multisampled);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA, X, Y, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, texcolorbuffer_multisampled, 0);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, X, Y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER::Framebuffer is not complete!\n";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &scr_tex);
	glBindTexture(GL_TEXTURE_2D, scr_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, X, Y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, scr_tex, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER::Intermediate framebuffer is not complete!\n";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
*/

void openglcode::framebuffer() {
	float border_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glGenFramebuffers(1, &fbo);
	glGenTextures(1, &depth_map);
	glBindTexture(GL_TEXTURE_2D, depth_map);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadow_x, shadow_y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer not complete!" << std::endl;
		// Handle the error (e.g., exit the application)
	}
}

void openglcode::depth_cubemap() {
	glGenFramebuffers(1, &fbo);
	glGenTextures(1, &depth_cube_map);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depth_cube_map);

	for (unsigned int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			shadow_x, shadow_y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_cube_map, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void openglcode::g_buffer() {
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	unsigned int rbo_depth;

	glGenFramebuffers(1, &gbo);
	glBindFramebuffer(GL_FRAMEBUFFER, gbo);

	glGenTextures(1, &gpos);
	glBindTexture(GL_TEXTURE_2D, gpos);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, X, Y, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gpos, 0);

	glGenTextures(1, &gnorm);
	glBindTexture(GL_TEXTURE_2D, gnorm);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, X, Y, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gnorm, 0);

	glGenTextures(1, &gcolor_spec);
	glBindTexture(GL_TEXTURE_2D, gcolor_spec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, X, Y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gcolor_spec, 0);

	glDrawBuffers(3, attachments);

	glGenRenderbuffers(1, &rbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, X, Y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "FRAMEBUFFER not COMPLETE!\n";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void openglcode::ssao() {
	glGenFramebuffers(1, &ssbo);
	glBindFramebuffer(GL_FRAMEBUFFER, ssbo);

	glGenTextures(1, &sscolor_buffer);
	glBindTexture(GL_TEXTURE_2D, sscolor_buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, X, Y, 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sscolor_buffer, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenFramebuffers(1, &ssbro);
	glBindFramebuffer(GL_FRAMEBUFFER, ssbro);
	glGenTextures(1, &sscolor_buffer_blur);
	glBindTexture(GL_TEXTURE_2D, sscolor_buffer_blur);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, X, Y, 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sscolor_buffer_blur, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void openglcode::hdrbuffer() {
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };

	glGenFramebuffers(1, &hdr_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, hdr_fbo);
	glGenTextures(2, color_buffer);

	for (unsigned int i = 0; i < 2; i++) {
		glBindTexture(GL_TEXTURE_2D, color_buffer[i]);
		//32bits isn't really necessary
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, X, Y, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, color_buffer[i], 0);
	}

	glGenRenderbuffers(1, &hdr_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, hdr_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, X, Y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, hdr_depth);

	glDrawBuffers(2, attachments);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "frame buffer2 not complete!\n";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenFramebuffers(2, pbo);
	glGenTextures(2, pbuffer);

	for (unsigned int i = 0; i < 2; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, pbo[i]);
		glBindTexture(GL_TEXTURE_2D, pbuffer[i]);
		//32bits isn't really necessary
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, X, Y, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pbuffer[i], 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "frame buffer2 not complete!\n";
		}

	}
}

void openglcode::render_scene(const Shader& shader) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(3.5f));
	shader.set_mat4("model", model);
	glDisable(GL_CULL_FACE);
	shader.set_int("reverse_normal", true);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	shader.set_int("reverse_normal", false);
	glEnable(GL_CULL_FACE);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.75f, -3.25f, 2.75f));
	model = glm::scale(model, glm::vec3(0.25f));
	shader.set_mat4("model", model);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, -0.75f, -1.0f));
	model = glm::scale(model, glm::vec3(0.25f));
	shader.set_mat4("model", model);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);


	/*
	model = glm::mat4(1.0f);
	model = glm::translate(model, light_pos);
	model = glm::scale(model, glm::vec3(0.1f));
	shader.set_mat4("model", model);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);*/
}

void openglcode::load_font() {
	//Controls how texture data is read from memory.
	//This code sets the read to 1 byte at a time without padding to represent the color. (default=4)
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (unsigned char c = 0; c < 128; c++) {
		if (FT_Load_Char(fface, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR::FREETYPE: Failed to load Glyph\n";
			continue;
		}

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 
			fface->glyph->bitmap.width, 
			fface->glyph->bitmap.rows, 
			0, GL_RED, GL_UNSIGNED_BYTE,
			fface->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			glm::ivec2(fface->glyph->bitmap.width, fface->glyph->bitmap.rows),
			glm::ivec2(fface->glyph->bitmap_left, fface->glyph->bitmap_top),
			fface->glyph->advance.x
		};
		characters.insert(std::pair<char, Character>(c, character));
	}

	FT_Done_Face(fface);
	FT_Done_FreeType(ft);
}

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void openglcode::process_input(GLFWwindow* window) {
	//커서 안보이게 하고 창화면에 가둠
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	//대각선 이동이 가능하도록 하기 위해 전부 if로
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.process_keyboard(FORWARD, delta_time);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.process_keyboard(BACKWARD, delta_time);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.process_keyboard(LEFT, delta_time);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.process_keyboard(RIGHT, delta_time);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		camera.process_keyboard(UP, delta_time);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		camera.process_keyboard(DOWN, delta_time);
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		std::cout << "EXIT\n";
		glfwSetWindowShouldClose(window, true);
	}
}

void mouse_callback(GLFWwindow* window, double x_pos, double y_pos) {
	float x = (float)x_pos;
	float y = (float)y_pos;

	if (first_mouse) {
		last_x = x_pos;
		last_y = y_pos;

		first_mouse = false;
	}
	float x_offset = x - last_x;
	float y_offset = last_y - y;

	last_x = x;
	last_y = y;

	camera.process_mouse_movement(x_offset, y_offset);
}

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset) {
	camera.process_mouse_scroll((float)y_offset);
}
/*
GLenum openglcode::_gl_check_error(const char* file, int line) {
	GLenum error_code;

	while ((error_code = glGetError()) != GL_NO_ERROR) {
		std::string error;

		switch (error_code) {
		case GL_INVALID_ENUM:
			error = "INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			error = "INALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			error = "INVALID_OPERATION";
			break;
		case GL_STACK_OVERFLOW:
			error = "STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			error = "STACK_UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			error = "OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			error = "INVALID_FRAMEBUFFER_OPERATION";
			break;
		default:
			error = "UNKNOWN_ERROR";
		}
		std::cout << error << " | " << file << " ( " << line << " ) " << std::endl;
	}

	return error_code;
}
*/
void APIENTRY gl_debug_output(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* user_param) {
	if (id == 131169 || id == 131218 || id == 131204) {
		return;
	}

	std::cout << "=================" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source) {
	case GL_DEBUG_SOURCE_API: std::cout << "Source: API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: std::cout << "Source: Window system";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader compiler";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY: std::cout << "Source: Third party";
		break;
	case GL_DEBUG_SOURCE_APPLICATION: std::cout << "Source: Application";
		break;
	case GL_DEBUG_SOURCE_OTHER: std::cout << "Source: Other";
		break;
	default:
		std::cout << "Source: Unknown";
	}
	std::cout << std::endl;

	switch (type) {
	case GL_DEBUG_TYPE_ERROR: std::cout << "Type: Error";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Depreacted behavior";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: std::cout << "Type: Undefined behavior";
		break;
	case GL_DEBUG_TYPE_PORTABILITY: std::cout << "Type: Portability";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE: std::cout << "Type: Performance";
		break;
	case GL_DEBUG_TYPE_MARKER: std::cout << "Type: Marker";
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP: std::cout << "Type: Push group";
		break;
	case GL_DEBUG_TYPE_POP_GROUP: std::cout << "Type: Pop group";
		break;
	case GL_DEBUG_TYPE_OTHER: std::cout << "Type: Other";
		break;
	default:
		std::cout << "Type: Unknown";
	}
	std::cout << std::endl;

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH: std::cout << "Severity: HIGH";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM: std::cout << "Severity: Medium";
		break;
	case GL_DEBUG_SEVERITY_LOW: std::cout << "Severity: Low";
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: Notification";
		break;
	default:
		std::cout << "Severity: Unknown";
	}

	std::cout << std::endl << std::endl;
}