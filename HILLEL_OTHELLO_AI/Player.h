
#ifndef _PLAYER_H
#define _PLAYER_H
#include "Board.h"
#include <time.h>
#include <string>

class Player{
private:
	char number;
	bool isHuman;
	float thinktime;
	int alphabeta(Board  &b,int depth,int alpha,int beta,double starttime);
public:
	Player(char num,bool human,double thinktime=0);
	void makeMove(Board &b);
	int getBestMove(Board  &b,int depth,double starttime);
};

#endif