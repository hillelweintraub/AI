
#ifndef _BOARD_H
#define _BOARD_H
#include <iostream>
#include <cstdlib>
#include <fstream>
#define PLAYER1 '1'
#define PLAYER2 '2'
#define EMPTY   '0'
using namespace std;



class Board
{
private:
	char board[8][8];
	bool moveList[8][8];
	char turn;
	int numLegalMoves;
	bool gameOver;
private:
	void printRow(int i);
	bool checkHorz(int i,int j);
	bool checkVert(int i,int j);
	bool checkDiags(int i,int j);
public:
	Board();
	Board(ifstream &input);
	void displayBoard();
	bool makeMove(int r, int c);
	void pass(bool printMessage=false);
	bool gameIsOver() const {return gameOver;}
	int getNumLegalMoves() const {return numLegalMoves;}
	bool isLegal(int r,int c) const {return moveList[r][c];}
	char getTurn() const {return turn;}
	void updateLegalMoves();
	int getUtility(char player);
	int getScore(char player) const;
	void flipturn() {turn=turn=='1'?'2':'1';}
	static int posvals[8][8];
};
#endif //_BOARD_H