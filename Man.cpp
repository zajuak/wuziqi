#include "Man.h"

void Man::init(Chess* chess)
{
	this->chess = chess;
}

void Man::go()
{
	MOUSEMSG msg;
	Chesspos pos;
	while (1) {
		msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN && msg.x>=146 &&msg.x<=743&&msg.y>=148&&msg.y<=747) {
			if (chess->click(msg.x, msg.y, &pos))
				break;
		}
		else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 595 && msg.x <= 797 && msg.y >= 775 && msg.y <= 821) {
			chess->remember();
				break;
		}
	}
	chess->putchess(&pos, black);
}
