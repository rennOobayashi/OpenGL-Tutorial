#include "openglcode.h"

enum GameState {
	GAME_ACTIVE, //0
	GAME_MENU,   //1
	GAME_WIN     //2
};

class game 
{
private:
	GameState state;
	bool keys[1024];
	unsigned int width, height;
public:
	game(unsigned int _width, unsigned int _height);
	~game() {} //Destructor
	void init();
	void process_input(float dt);
	void update(float dt);
	void render();
};
