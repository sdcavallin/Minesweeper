#include "Tile.h"

Tile::Tile(bool isMine) {
	mine = isMine;
	revealed = 0;
	flagged = 0;
	adjMines = 0;
}
