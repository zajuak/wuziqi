#pragma once
#include <graphics.h>
#include <vector>
using namespace std;
typedef enum {
	white = -1,//����
	black = 1//����
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
	//��ʼ������
	void init();
	void init1();
	//����
	bool click(int x, int y, Chesspos* pos);
	//����
	void putchess(Chesspos* pos, chess_kind kind);
	//�ж�λ��״̬
	int getchess(Chesspos* pos);
	int getchess(int row, int col);
	//�жϽ���
	bool checkover();
	void remember();
	
private:
	int boardsize; //���̳ߴ�
	int margin_x;
	int margin_y;
	float chesssize;
	IMAGE board;
	IMAGE chessblackImg;
	IMAGE chesswhiteImg;
	IMAGE clue;
	//�洢��ǰ���̺�����״̬ ����1����-1����0
	vector<vector<int>> chessmap;
	//��ʾ���巽 ����1����0
	Chesspos  lastPos;
	bool playerflag;
	void updateGamemap(Chesspos* pos);
	bool checkwin();
};

