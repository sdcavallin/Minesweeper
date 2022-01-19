#include "Board.h"

Board::Board() {
	ifstream file("boards/config.cfg");

	cols = 0, rows = 0, mines = 0, debug = 0, lost = 0, won = 0, tilesLeft = 0;
	if (file.is_open()) {
		file >> cols;
		file >> rows;
		file >> mines;
	}
	cmines = mines;
	counter = mines;
	tiles = rows * cols;
	board = nullptr;
	
	width = cols * 32;
	height = rows * 32 + 88;

	generate();
	file.close();
} 

void Board::generate() {
	delete[] board;
	board = new Tile[tiles];

	lost = 0, won = 0;
	mines = cmines;
	counter = mines;
	tilesLeft = tiles - mines;

	set<int> rands;
	while (rands.size() < mines)
	{
		rands.insert(Random::Int(0, tiles - 1));
	}

	for (set<int>::iterator it = rands.begin(); it != rands.end(); it++) {
		board[*it].mine = 1;
	}

	setAdj();
	
}

void Board::generate(int preset) {
	string path = "boards/testboard" + to_string(preset) + ".brd";
	ifstream file(path);

	delete[] board;
	board = new Tile[tiles];

	lost = 0, won = 0;
	mines = 0;

	if (file.is_open()) {
		string line;
		int row = 0;
		while(getline(file, line)) {
			for (int i = 0; i < cols; i++) {
				if (line[i] == '1') {
					board[i + (row*cols)].mine = 1;
					mines++;
				}
			}
			row++;
		}
	}

	counter = mines;
	tilesLeft = tiles - mines;

	setAdj();
	file.close();
}

void Board::draw(sf::RenderWindow& window) {
	drawTiles(window);
	drawFlags(window); 
	drawMines(window);
	drawButtons(window);
	drawCounter(window);
}

void Board::drawTiles(sf::RenderWindow& window) {
	sf::Sprite hidden(Textures::get("tile_hidden"));
	sf::Sprite revealed(Textures::get("tile_revealed"));

	sf::Sprite tile[9];
	tile[0] = sf::Sprite(Textures::get("tile_revealed"));
	for (int i = 1; i < 9; i++) {
		tile[i] = sf::Sprite(Textures::get("number_" + to_string(i)));
	}

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			revealed.setPosition(j * 32, i * 32);
			window.draw(revealed);
		}
	}

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			Tile& curr = board[(i * cols) + j];
			if (curr.revealed) {
				tile[curr.adjMines].setPosition(j * 32, i * 32);
				window.draw(tile[curr.adjMines]);
			}
			else {
				hidden.setPosition(j * 32, i * 32);
				window.draw(hidden);
			}
		}
	}
	
}

void Board::drawFlags(sf::RenderWindow& window) {
	sf::Sprite flag(Textures::get("flag"));
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			Tile& curr = board[(i * cols) + j];
			if (won) {
				if (curr.mine && !curr.flagged) {
					curr.flagged = true;
					counter--;
				}
			}
			if (curr.flagged) {
				flag.setPosition(j * 32, i * 32);
				window.draw(flag);
			}
		}
	}
}

void Board::drawMines(sf::RenderWindow& window) {
	sf::Sprite mine(Textures::get("mine"));
	if (debug || lost) {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				if (board[(i * cols) + j].mine) {
					mine.setPosition(j * 32, i * 32);
					window.draw(mine);
				}
			}
		}
	}
}

void Board::drawButtons(sf::RenderWindow& window) {
	//buttons.clear();

	sf::Sprite smiley(Textures::get("face_happy"));
	if (lost) {
		smiley.setTexture(Textures::get("face_lose"));
	}
	if (won) {
		smiley.setTexture(Textures::get("face_win"));
	}
	smiley.setPosition(width / 2 - smiley.getGlobalBounds().width / 2, height - 88);
	window.draw(smiley);

	sf::Sprite debug(Textures::get("debug"));
	debug.setPosition(width - 4 * blen, height - 88);
	window.draw(debug);

	sf::Sprite test1(Textures::get("test_1"));
	test1.setPosition(width - 3 * blen, height - 88);
	window.draw(test1);

	sf::Sprite test2(Textures::get("test_2"));
	test2.setPosition(width - 2 * blen, height - 88);
	window.draw(test2);

	sf::Sprite test3(Textures::get("test_3"));
	test3.setPosition(width - blen, height - 88);
	window.draw(test3);

	if (buttons.size() == 0) {
		buttons.push_back(smiley);
		buttons.push_back(debug);
		buttons.push_back(test1);
		buttons.push_back(test2);
		buttons.push_back(test3);
	}

}

void Board::drawCounter(sf::RenderWindow& window) {
	sf::Sprite d[3];

	int ctr = counter;
	if (ctr < 0) {
		ctr = -ctr;
	}

	d[0].setTexture(Textures::num(ctr / 100));
	ctr %= 100;
	d[1].setTexture(Textures::num(ctr / 10));
	ctr %= 10;
	d[2].setTexture(Textures::num(ctr));

	if (counter < 0) {
		d[0].setTexture(Textures::num(10));
	}

	for (int i = 0; i < 3; i++) {
		d[i].setPosition(i * 21, height - 88);
		window.draw(d[i]);
	}
}

void Board::click(sf::Vector2i& pos, bool right) {
	if (pos.y < height - 88) {
		//cout << "Tile pressed" << endl;
		int x = pos.x / 32, y = pos.y / 32;

		int number = y * cols + x;
		Tile& tile = board[number];
		if (right && !tile.revealed) {
			if (tile.flagged) {
				counter++;
			}
			else {
				counter--;
			}
			tile.flagged = !tile.flagged;
		}
		else if (!right && !lost && !won && !tile.flagged && !tile.revealed) {
			reveal(tile);
		}
	}

	
	sf::FloatRect smiley(buttons[0].getPosition().x, buttons[0].getPosition().y, 64.0f, 64.0f);
	if (smiley.contains(pos.x, pos.y)) {
		//cout << "Smiley pressed" << endl;
		generate();
	}

	sf::FloatRect viewMines(buttons[1].getPosition().x, buttons[1].getPosition().y, 64.0f, 64.0f);
	if (viewMines.contains(pos.x, pos.y)) {
		//cout << "Debug pressed" << endl;
		debug = !debug;
	}

	for (int i = 2; i < 5; i++) {
		sf::FloatRect button(buttons[i].getPosition().x, buttons[i].getPosition().y, 64.0f, 64.0f);
		if (button.contains(pos.x, pos.y)) {
			//cout << "Test #" << (i - 1) << " pressed" << endl;
			generate(i - 1);
		}
	}

}

void Board::reveal(Tile& tile) {
	tile.revealed = 1;

	if (tile.mine) {
		lost = true;
		return;
	}

	tilesLeft--;
	if (tilesLeft == 0) {
		won = true;
		return;
	}
	
	if (tile.adjMines == 0) {
		for (Tile* adjacent : tile.adj) {
			if (!adjacent->revealed && !adjacent->flagged) {
				reveal(*adjacent);
			}
		}
	}

}

void Board::setAdj() {
	int pos = 0;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			Tile& curr = board[pos];
			
			for (int dx = -1; dx <= 1; dx++) {
				for (int dy = -1; dy <= 1; dy++) {
					if (dx == 0 && dy == 0) continue;
					
					if (i + dx < rows && i + dx >= 0 && j + dy < cols && j + dy >= 0) { 
						curr.adj.push_back(&board[((i+dx) * cols) + (j+dy)]);
					}
					
				}
			}

			for (Tile* adjacent : curr.adj) {
				if (adjacent->mine) {
					curr.adjMines++;
				}
			}

			pos++;
		}

	}
}

Board::~Board() {
	delete[] board;
}