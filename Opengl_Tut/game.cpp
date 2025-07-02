#include "game.h"

game::game(unsigned int _width, unsigned int _height) {
	width = _width;
	height = _height;
}

void game::update(float dt) {
	openglcode opengl;

	try {
		opengl.init();
		opengl.run();
	}
	catch (int error_num) {
		switch (error_num) {
		case 1:
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
			break;
		case 2:
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
			break;
		case 3:
			std::cout << "ERROR::FREETYPE: Failed to load Glyph(text load)" << std::endl;
			break;
		}
		return;
	}
	return;
}
