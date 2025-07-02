#include <iostream>
#include "game.h"
#include "openglcode.h"

int main()
{
	game g(1280, 720);

	g.update(60.0f);
}