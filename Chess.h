#pragma once
#include <graphics.h>
#include <vector>
using namespace std;
typedef enum {
	white = -1,//°×Æå
	black = 1//ºÚÆå
}chess_kind;
struct Chesspos {
	int row;
	int col;
	Chesspos(int r = 0, int c = 0) :row(r), col(c) {}
};
class Chess
{
public:
	Chess(int boardsize, int marginx, int marginy, float chesssize);
	//³õÊ¼»¯ÆåÅÌ
	void init();
	void init1();
	//ÏÂÆå
	bool click(int x, int y, Chesspos* pos);
	//Âä×Ó
	void putchess(Chesspos* pos, chess_kind kind);
	//ÅĞ¶ÏÎ»ÖÃ×´Ì¬
	int getchess(Chesspos* pos);
	int getchess(int row, int col);
	//ÅĞ¶Ï½áÊø
	bool checkover();
	void remember();
	
private:
	int boardsize; //ÆåÅÌ³ß´ç
	int margin_x;
	int margin_y;
	float chesssize;
	IMAGE board;
	IMAGE chessblackImg;
	IMAGE chesswhiteImg;
	IMAGE clue;
	//´æ´¢µ±Ç°ÆåÅÌºÍÆå×Ó×´Ì¬ ºÚÆå1°×Æå-1·ñÔò0
	vector<vector<int>> chessmap;
	//±íÊ¾ÏÂÆå·½ ºÚÆå1°×Æå0
	Chesspos  lastPos;
	bool playerflag;
	void updateGamemap(Chesspos* pos);
	bool checkwin();
};

