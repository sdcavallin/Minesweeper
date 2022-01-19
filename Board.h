#pragma once
#include <string>
#include <fstream>
#include <set>
#include <iostream>
#include "Tile.h"
#include "Random.h"
#include "Textures.h"
#include <SFML/Graphics.hpp>

using namespace std;


class Board {
	Tile* board;
	const unsigned int blen = 64;
	vector<sf::Sprite> buttons;

	void drawButtons(sf::RenderWindow& window);
	void drawCounter(sf::RenderWindow& window);
	void drawTiles(sf::RenderWindow& window);
	void drawFlags(sf::RenderWindow& window);
	void drawMines(sf::RenderWindow& window);
	void setAdj();
public:
	int rows, cols, mines, cmines, tiles, width, height;
	bool debug, lost, won;
	int counter, tilesLeft;

	Board();

	void generate();
	void generate(int preset);
	void draw(sf::RenderWindow& window);

	void click(sf::Vector2i& pos, bool right);
	void reveal(Tile& tile);

	~Board();
};

