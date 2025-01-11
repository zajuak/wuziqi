#include "Chess.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include <conio.h>
#include<fstream>
#include<iostream>
Chess::Chess(int boardsize, int marginx, int marginy, float chesssize)
{
	this-> boardsize = boardsize;
	this->margin_x = marginx;
	this->margin_y = marginy;
	this->chesssize = chesssize;
	playerflag = black;
	for (int i = 0;i < boardsize;i++) {
		vector<int>row;
		for (int j = 0;j < boardsize;j++) {
			row.push_back(0);
		}
		chessmap.push_back(row);
	}
}
void putimagePNG(int x, int y, IMAGE* picture) //x为载入图片的X坐标，y为Y坐标
{
	// 变量初始化
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //获取picture的显存指针
	int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
	int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
	int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
	int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
	int dstX = 0;    //在显存里像素的角标

	// 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + x) + (iy + y) * graphWidth; //在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
			}
		}
	}
}
void Chess::init()
{	
	loadimage(&board, "res/board.jpg");
	putimage(0, 0, &board);
	mciSendString("play res/start.wav", 0, 0, 0);
	loadimage(&chessblackImg, "res/black.png", chesssize, chesssize, true);
	loadimage(&chesswhiteImg, "res/white.png", chesssize, chesssize, true);
	for (int i = 0;i < chessmap.size();i++) {
		for (int j = 0;j < chessmap[i].size();j++) {
			chessmap[i][j] = 0;
		}
	}
	playerflag = true;
}

void Chess::init1()
{
	loadimage(&clue, "res/1.jpg");
	putimage(169, 357, &clue);


	ifstream ifs;
	string s;
	cout << "请输入要打开的存档";
	cin >> s;
	s += ".txt";
	ifs.open(s, ios::in);
	for (int i = 0;i < chessmap.size();i++) {
		for (int j = 0;j < chessmap[i].size();j++) {
			ifs >> chessmap[i][j];
		}
	}
	ifs.close();
	loadimage(&board, "res/board.jpg");
	putimage(0, 0, &board);

	loadimage(&chessblackImg, "res/black.png", chesssize, chesssize, true);
	loadimage(&chesswhiteImg, "res/white.png", chesssize, chesssize, true);
	for (int i = 0;i < 15;i++) {
		for (int j = 0;j < 15;j++) {
			int x = margin_x + j * chesssize - 0.5 * chesssize;
			int y = margin_y + i * chesssize - 0.5 * chesssize;
			if (chessmap[i][j] == 1) {
				putimagePNG(x, y, &chessblackImg);
			}
			else if(chessmap[i][j] == -1){
				putimagePNG(x, y, &chesswhiteImg);
			}
		}
	}
	mciSendString("play res/start.wav", 0, 0, 0);
	playerflag = true;
}

bool Chess::click(int x, int y, Chesspos* pos)
{
	int col = (x - margin_x) / chesssize;
	int row = (y - margin_y) / chesssize;
	int lefttopx = margin_x + chesssize * col;
	int lefttopy = margin_y + chesssize * row;
	double offset = chesssize * 0.4;//模糊上限
	double len;
	//左上
		len = sqrt((x - lefttopx) * (x - lefttopx) + (y - lefttopy) * (y - lefttopy));
		if (len < offset) {
			pos->row = row;
			pos->col = col;
			if (chessmap[pos->row][pos->col] == 0) {
				return 1;
			}
		}
		//右上
		len = sqrt((x - lefttopx - chesssize) * (x - lefttopx - chesssize) + (y - lefttopy) * (y - lefttopy));
		if (len < offset) {
			pos->row = row;
			pos->col = col + 1;
			if (chessmap[pos->row][pos->col] == 0) {
				return 1;
			}
		}

		// 距离左下角的距离
		len = sqrt((x - lefttopx) * (x - lefttopx) + (y - lefttopy - chesssize) * (y - lefttopy - chesssize));
		if (len < offset) {
			pos->row = row + 1;
			pos->col = col;
			if (chessmap[pos->row][pos->col] == 0) {
				return 1;
			}
		}

		// 距离右下角的距离
		len = sqrt((x - lefttopx - chesssize) * (x - lefttopx - chesssize) + (y - lefttopy - chesssize) * (y - lefttopy - chesssize));
		if (len < offset) {
			pos->row = row + 1;
			pos->col = col + 1;

			if (chessmap[pos->row][pos->col] == 0) {
				return 1;
			}
		}
		return 0;
}


void Chess::putchess(Chesspos *pos, chess_kind kind)
{
	mciSendString("play res/down7.wav", 0, 0, 0);
	int x = margin_x + pos->col * chesssize - 0.5 * chesssize;
	int y = margin_y + pos->row * chesssize - 0.5 * chesssize;
	if (kind == white) {
		putimagePNG(x, y, &chesswhiteImg);
	}
	else {
		putimagePNG(x, y, &chessblackImg);
	}
	updateGamemap(pos);
}

int Chess::getchess(Chesspos* pos)
{
	return chessmap[pos->row][pos->col];
}

int Chess::getchess(int row, int col)
{
	return chessmap[row][col];
}

bool Chess::checkover()
{
	if (checkwin()) {
		Sleep(1000);
		if (playerflag == 0) {
			mciSendString("play res/不错.mp3", 0, 0, 0);
			loadimage(0, "res/胜利.jpg");
		}
		else {
			mciSendString("play res/失败.mp3", 0, 0, 0);
			loadimage(0, "res/失败.jpg");
		}

		_getch();
		return 1;
	}
	return 0;
}

void Chess::remember()
{
	string s;
	cout << "请输入存档名称";
	cin >> s;
	s += ".txt";
	ofstream file(s, fstream::out);
	for (int i = 0;i < chessmap.size();i++) {
		for (int j = 0;j < chessmap[i].size();j++) {
			file << chessmap[i][j] << " ";
		}
		file << endl;
	}
	exit(0);
}

void Chess::updateGamemap(Chesspos* pos)
{
	lastPos = *pos;
	chessmap[pos->row][pos->col] = playerflag ? 1 : -1;
	playerflag = !playerflag;
}

bool Chess::checkwin()
{
	int row = lastPos.row;
	int col = lastPos.col;

	for (int i = 0; i < 5; i++)
	{
		// 往左5个，往右匹配4个子，20种情况
		if (col - i >= 0 &&
			col - i + 4 < boardsize &&
			chessmap[row][col - i] == chessmap[row][col - i + 1] &&
			chessmap[row][col - i] == chessmap[row][col - i + 2] &&
			chessmap[row][col - i] == chessmap[row][col - i + 3] &&
			chessmap[row][col - i] == chessmap[row][col - i + 4])
			return true; 
	}

	// 竖直方向(上下延伸4个)
	for (int i = 0; i < 5; i++)
	{
		if (row - i >= 0 &&
			row - i + 4 < boardsize &&
			chessmap[row - i][col] == chessmap[row - i + 1][col] &&
			chessmap[row - i][col] == chessmap[row - i + 2][col] &&
			chessmap[row - i][col] == chessmap[row - i + 3][col] &&
			chessmap[row - i][col] == chessmap[row - i + 4][col])
			return true;
	}

	// “/"方向
	for (int i = 0; i < 5; i++)
	{
		if (row + i < boardsize &&
			
			row + i - 4 >= 0 &&
			col - i >= 0 &&
			col - i + 4 < boardsize &&
			// 第[row+i]行，第[col-i]的棋子，与右上方连续4个棋子都相同
			chessmap[row + i][col - i] == chessmap[row + i - 1][col - i + 1] &&
			chessmap[row + i][col - i] == chessmap[row + i - 2][col - i + 2] &&
			chessmap[row + i][col - i] == chessmap[row + i - 3][col - i + 3] &&
			chessmap[row + i][col - i] == chessmap[row + i - 4][col - i + 4])
			return true;
	}

	// “\“ 方向
	for (int i = 0; i < 5; i++)
	{
		// 第[row+i]行，第[col-i]的棋子，与右下方连续4个棋子都相同
		if (row - i >= 0 &&
			row - i + 4 < boardsize &&
			col - i >= 0 &&
			col - i + 4 < boardsize &&
			chessmap[row - i][col - i] == chessmap[row - i + 1][col - i + 1] &&
			chessmap[row - i][col - i] == chessmap[row - i + 2][col - i + 2] &&
			chessmap[row - i][col - i] == chessmap[row - i + 3][col - i + 3] &&
			chessmap[row - i][col - i] == chessmap[row - i + 4][col - i + 4])
			return true;
	}

	return false;
}
