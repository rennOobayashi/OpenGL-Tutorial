#include "gamelevel.h"

void GameLevel::init(std::vector<std::vector<unsigned int>> tile_data, unsigned int level_width, unsigned int level_height) {
	unsigned int width = tile_data[0].size(); //x
	unsigned int height = tile_data.size(); //y
	//Casting reliably with static_cast
	float unit_width = level_width / static_cast<float>(width);
	float unit_height = level_height / height;

	for (unsigned int y = 0; y < height; ++y) {
		for (unsigned int x = 0; x < width; ++x) {
			if (tile_data[y][x] == 1) {
				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				GameObject obj(pos, size, ResourceManager::get_texture("solid_block"), glm::vec3(0.8f, 0.8f, 0.7f));
				obj.is_solid = true;
				bricks.push_back(obj);
			}
			else if (tile_data[y][x] > 1) {
				glm::vec3 color = glm::vec3(1.0f);
				
				switch (tile_data[y][x]) {
					case 2: color = glm::vec3(0.2f, 0.6f, 1.0f);
						break;
					case 3: color = glm::vec3(0.1f, 0.7f, 0.2f);
						break;
					case 4: color = glm::vec3(0.4f, 0.8f, 0.8f);
						break;
					case 5: color = glm::vec3(0.4f, 0.3f, 0.0f);
						break;
					default: std::cout << "Wrong tile code" << std::endl;
				}

				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height); 
				GameObject obj(pos, size, ResourceManager::get_texture("block"), color);
				bricks.push_back(obj);
			}
		}
	}
}

void GameLevel::load(const char* file, unsigned int level_width, unsigned int level_height) {
	//Clear old datas
	bricks.clear();

	//Load from file
	unsigned int tile_code;
	GameLevel level;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<unsigned int>> tile_data;

	//File processing
	if (fstream) {
		//Read one line at a time from the level file.
		while (std::getline(fstream, line)) {
			std::istringstream sstream(line);
			std::vector<unsigned int> row;
			
			//Read it separated by spaces.
			while (sstream >> tile_code) {
				row.push_back(tile_code);
			}
			tile_data.push_back(row);
		}

		if (tile_data.size() > 0) {
			init(tile_data, level_width, level_height);
		}
	}
}

void GameLevel::draw(SpriteRenderer &renderer) {
	for (GameObject& tile : bricks) {
		if (!tile.destroyed) {
			tile.draw(renderer);
		}
	}
}

bool GameLevel::is_completed() {
	for (GameObject& tile : bricks) {
		//If theres not even one broken, its not over
		if (!tile.destroyed && !tile.is_solid) {
			return false;
		}
	}

	return true;
}