#pragma once
#include <vector>

using namespace std;

class Tile {
public:
	bool mine, revealed, flagged;
	int adjMines;
	vector<Tile*> adj;

	Tile(bool isMine = 0);
};

