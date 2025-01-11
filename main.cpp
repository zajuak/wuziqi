#include <iostream>
#include "ChessGame.h"
#include <fstream>

int main(void) {
	Chess chess(15,159,166,40.75);
	Man man;
	AI ai;
	ChessGame game(&man, &ai, &chess);
	game.begin();
	

	return 0;
}