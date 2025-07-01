#include <iostream>
#include "game.h"
#include "openglcode.h"

int main()
{
#if false
	openglcode run;

	try {
		run.init();
		run.set_n_run();
	}
	catch (int error_num) {
		switch (error_num) {
		case 1 :
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
#endif

#if 1
	game g(1280, 720);

	g.update(60.0f);
#endif
}