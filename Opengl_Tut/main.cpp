#include <iostream>
#include "game.h"
#define X 1280
#define Y 720

int main()
{
	Game game(X, Y);

	try {
		game.init();
		game.update();
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
		return -1;
	}

	return 0;
}