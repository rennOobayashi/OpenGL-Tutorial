#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include <glad/glad.h>
#include "stb_image.h"

#include "texture.h"
#include "shader.h"

class ResourceManager
{
public:
	static std::map<std::string, Shader> shaders;
	static std::map<std::string, Texture> textures;

	static Shader load_shader(const char *vshader_file, const char * fshader_file, const char *gshader_file, std::string name);
	static Shader get_shader(std::string name);


	static Texture load_texture(const char* file, bool alpha, std::string name);
	static Texture get_texture(std::string name);
	static void clear();

private:
	ResourceManager() {}
	static Shader load_shader_from_file(const char *vshader_file, const char * fshader_file, const char *gshader_file = nullptr);
	static Texture load_texture_from_file(const char* file, bool alpha);
};

#endif