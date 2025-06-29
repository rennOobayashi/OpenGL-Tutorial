#include "shader.h"
#include "model.h"
#include "camera.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <random>
#include <fstream>
#include <iostream>
#include <vector>
#define X 1280
#define Y 720
#define gl_check_error() _gl_check_error(__FILE__, __LINE__)

struct Character {
	unsigned int texture_id;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advence;
};


class openglcode
{
private:
	std::map<unsigned char, Character> characters;
	const unsigned int shadow_x = 1024, shadow_y = 1024;
	GLFWwindow* window;
	glm::vec3 light_pos;
	glm::vec3 light_color;
	glm::vec3 light_dir;
	FT_Library ft;
	FT_Face fface;
	unsigned int vao, vbo, color_buffer[2], sao, sbo, fbo, depth_map, qao, qbo, depth_cube_map, hdr_fbo, hdr_depth, pbo[2], pbuffer[2];
	unsigned int gbo, gpos, gnorm, gcolor_spec, noise_texture;
	unsigned int ssbo, sscolor_buffer, ssbro, sscolor_buffer_blur;
	unsigned int tao, tbo;
	unsigned int capture_fbo, capture_rbo;
	unsigned int env_cubemap;
	unsigned int irradiance_map;
	unsigned int prefilter_map;
	unsigned int brdf_texture;
	unsigned int sphere_vao, eao, ebo;
	unsigned int index_cnt;
	char info_log[512];
	unsigned int texture1, texture2;
	unsigned int albedo_tex, metal_tex, nor_tex, rough_tex, ao_tex, hdr_texture;
	unsigned int cubemap_texture;
	int flags;
	unsigned char* data;
	float delta_time;
	float last_frame;
	float exposure;
	float *f_data;

	void process_input(GLFWwindow* window);
	void draw_square();
	void draw_sphere();
	void draw_skybox(Shader hdr_shader, Shader irradiance_shader, Shader prefilter_shader, Shader brdf_shader);
	void framebuffer();
	void hdrbuffer();
	void depth_cubemap();
	void g_buffer();
	void ssao();
	void render_scene(const Shader &shader);
	unsigned int load_cubemap(std::vector<std::string> faces);
	unsigned int load_texture(char const* path);
	GLenum _gl_check_error(const char* file, int line);
	void load_font();
	void render_text(Shader& s, std::string text, float x, float y, float scale, glm::vec3 color);
public:
	void init();
	void set_n_run();
};