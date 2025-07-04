#include "resourcemanager.h"

// static variables -> instasnce
std::map<std::string, Texture> ResourceManager::textures;
std::map <std::string, Shader> ResourceManager::shaders;

Shader ResourceManager::load_shader(const char* vshader_file, const char* fshader_file, const char* gshader_file, std::string name) {
	shaders[name] = load_shader_from_file(vshader_file, fshader_file, gshader_file);
	return shaders[name];
}

Shader ResourceManager::get_shader(std::string name) {
		return shaders[name];
}

Texture ResourceManager::load_texture(const char *file, bool alpha, std::string name) {
	textures[name] = load_texture_from_file(file, alpha);
	return textures[name];
}

Texture ResourceManager::get_texture(std::string name) {
	Texture t;
	try {
		t = textures[name];
		return t;
	}
	catch (std::out_of_range& e) {
		std::cout << "Shader not found: " << name << std::endl;
		return t;
	}
}

void ResourceManager::clear() {
	for (auto iter : shaders) {
		//first - name, second - shader
		glDeleteProgram(iter.second.id);
	}

	for (auto iter : textures) {
		glDeleteTextures(1, &iter.second.id);
	}
}

Shader ResourceManager::load_shader_from_file(const char* vshader_file, const char* fshader_file, const char* gshader_file) {
	std::string vertex_code;
	std::string fragment_code;
	std::string geometry_code;

	try {
		std::ifstream vertex_shader_file(vshader_file);
		std::ifstream fragment_shader_file(fshader_file);
		std::stringstream vshader_stream, fshader_stream;

		vshader_stream << vertex_shader_file.rdbuf();
		fshader_stream << fragment_shader_file.rdbuf();

		vertex_shader_file.close();
		fragment_shader_file.close();

		vertex_code = vshader_stream.str();
		fragment_code = fshader_stream.str();

		if (gshader_file != nullptr) {
			std::ifstream gemetry_shader_file(gshader_file);
			std::stringstream gshader_stream;

			gshader_stream << gemetry_shader_file.rdbuf();

			geometry_code = gshader_stream.str();
		}
	}
	catch (std::exception e) {
		std::cout << "ERROR::SHADER::Failed to read shader files" << std::endl;
	}

	const char* vshader_code = vertex_code.c_str();
	const char* fshader_code = fragment_code.c_str();
	const char* gshader_code = geometry_code.c_str();

	Shader shader;
	shader.compile(vshader_code, fshader_code, gshader_file != nullptr ? gshader_code : nullptr);

	return shader;
}

Texture ResourceManager::load_texture_from_file(const char* file, bool alpha) {
	Texture texture;
	int width, height, nr_channels;
	unsigned char* data;

	if (alpha) {
		texture.internal_format = GL_RGBA;
		texture.image_format = GL_RGBA;
	}

	data = stbi_load(file, &width, &height, &nr_channels, 0);

	texture.generate(width, height, data);

	stbi_image_free(data);

	return texture;
}