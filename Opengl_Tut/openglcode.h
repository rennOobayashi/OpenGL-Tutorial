#ifndef OPENGLCODE_H
#define OPENGLCODE_H

#include "shader.h"
#include "model.h"
#include "camera.h"
#include "spriterenderer.h"
#include "texture.h"

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

class openglcode
{
private:
	GLFWwindow* window;
public:
	void init();
	void run();
};
#endif