#pragma once
#include "Man.h"
#include "AI.h"
#include "Chess.h"
class ChessGame
{
public:
	ChessGame (Man*, AI*, Chess*);
	void play0();
	void play1();
	void begin();
private:
	Man*  man;
	AI*  ai;
	Chess*  chess;
};



