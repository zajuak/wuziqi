#include "ChessGame.h"
#include <fstream>

ChessGame::ChessGame(Man* man, AI* ai, Chess* chess) {
	this->man = man;
	this->ai = ai;
	this->chess = chess;
	ai->init(chess);
	man->init(chess);
}
//对局开始
void ChessGame::play0() {
	chess->init();
	while (1) {
		man->go();
		if (chess->checkover()) {
			chess->remember();
			begin();
			continue;
		}
		Sleep(500);
		ai->go();
		if (chess->checkover()) {
			chess->remember();
			begin();
			continue;
		}
	}
}
void ChessGame::play1() {
	chess->init1();
	while (1) {		
		man->go();
		if (chess->checkover()) {
			chess->remember();
			begin();
			continue;
		}
		Sleep(500);
		ai->go();
		if (chess->checkover()) {
			chess->remember();
			begin();
			continue;
		}
	}
}

void ChessGame::begin()
{
	initgraph(899, 899);
	loadimage(0, "res/菜单.jpg");
	MOUSEMSG msg;
	Chesspos pos;
	while (1) {
		msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN) {
			if (msg.x > 305 && msg.x < 563 && msg.y>653 && msg.y < 709) {

				play0();
				break;
			}
			else if (msg.x > 261 && msg.x < 612 && msg.y>770 && msg.y < 830) {
				play1();
				break;
			}
			
		}

	}
}

