

#include "Board.h"
#include "Player.h"
#include <unistd.h>
using namespace std;

void getInputFILE(ifstream &input){
	string filename;
	cout << "Enter name of input file: ";
	cin >> filename;

	input.open(filename.c_str());
	if (!input) {
    	cerr << "Error: could not open " << filename << endl;
    	exit(1);
	}
}

void initPlayer(Player *&p,char num){
	char resp;
	while(1){
		cout<<"Will player"<<num<<" be a human(y/n)?";
		cin>>resp;
		if(resp=='y'){
			p= new Player(num,true);
			break;
		}
		if(resp=='n'){
			string thinktime;
			int timelim;
			while(1){
				cout<<"Enter a time limit in seconds for the computer to think (1-59):";
				cin>>thinktime;
				timelim=atoi(thinktime.c_str());
				if(timelim>0 && timelim<60) break; 
			}
			p= new Player(num,false,(double)timelim);
			break;
		}
		cout<<"Invalid response"<<endl;
	}
}

void initGame(Player *&p1, Player *&p2,Board *&b){
	//seed random # generator
	time_t seconds;
	time(&seconds);
	srand((unsigned int)seconds);
	initPlayer(p1,'1');
	initPlayer(p2,'2');
	char resp;
	while(1){
		cout<<"Do you want to load a game(y/n)?";
		cin>>resp;
		if(resp=='y'){
			ifstream input;
			getInputFILE(input);
			b= new Board(input);
			break;
		}
		if(resp=='n'){
			b= new Board();
			break;
		}
		cout<<"Invalid response"<<endl;
	}
}
bool gameOverCheck(Board *b){
	if(b->gameIsOver()){
		cout<<"Game over!!!"<<endl;
		int s1=b->getScore('1'), s2=b->getScore('2');
		cout<<"player1 scored "<<s1<<" points"<<endl;
		cout<<"player2 scored "<<s2<<" points"<<endl;
		if(s1==s2){
			cout<<"The game is a draw"<<endl;
		}else{
			cout<<(s1>s2?"Player1 wins!!!":"Player2 wins!!!")<<endl;
		}
		return true;
	}
	return false;
}

int main(int argc, char const *argv[]){
	Player *p1,*p2;
	Board *b;
	initGame(p1,p2,b);

	while(1){
		b->displayBoard();
		if(b->getTurn()=='1'){
			p1->makeMove(*b);
			if(gameOverCheck(b)) break;
			b->displayBoard();
		}
		p2->makeMove(*b);
		if(gameOverCheck(b)) break;
		//usleep(1000000);
	}
	return 0;
}
