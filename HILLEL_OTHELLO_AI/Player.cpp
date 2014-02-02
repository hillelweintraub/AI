
#include "Player.h"
#include "Board.h"
#include <sys/time.h>

Player::Player(char num,bool human,double time){
	number=num;
	isHuman=human;
	thinktime=time;
}

void Player::makeMove(Board &b){
	if(b.getNumLegalMoves()==0) {
		b.pass(true);
		return;
	}
	
	if(isHuman){
		int r=-1,c=-1;
		string inputstr;
		while(!(r>=0 && r<8 && c>=0 && c<8 && b.makeMove(r,c))){
			cout<<"Player "<<number<<" make a move ([row][col]):"; //prompt player
			cin>>inputstr;
			r=inputstr[0]-'0';
			c=inputstr[1]-'0';
		}
		cout<<endl<<endl;
	}else{
		if(b.getNumLegalMoves()==1){
			for(int r=0;r<8;r++){
				for(int c=0;c<8;c++){
					if(b.makeMove(r,c)){
						cout<<"Computer made its only legal move <"<<r<<","<<c<<">"<<endl<<endl;
						return;
					}
				}
			}
		}
		struct timeval stime,currtime;
		gettimeofday(&stime,NULL);
		double starttime=(double)stime.tv_sec+(double)stime.tv_usec/1000000.0;
		double etime=0;

		int move,depth;
		for(depth=1;;depth++){
			if(depth==30) break;
			int tmpmove=getBestMove(b,depth,starttime);
			if(tmpmove==-1) break;
			move=tmpmove;
			gettimeofday(&currtime,NULL);
			etime=((double)currtime.tv_sec+(double)currtime.tv_usec/1000000.0)-starttime;
			if(etime>thinktime/1.9) break;
		}
		gettimeofday(&currtime,NULL);
		etime=((double)currtime.tv_sec+(double)currtime.tv_usec/1000000.0)-starttime;
		int r=move/10;
		int c=move%10;
		if(depth<15) cout<<"Computer searched to depth "<<depth-1<<endl;
		else cout<<"Computer searched to the end of the search tree"<<endl;
		cout<<"search took "<<etime<<" seconds"<<endl;
		cout<<"Computer chose <"<<r<<","<<c<<">"<<endl<<endl;
		b.makeMove(r,c);
	}
}

int Player::alphabeta(Board  &b,int depth,int alpha,int beta,double starttime){
	struct timeval currtime;
	gettimeofday(&currtime,NULL);
	double etime=((double)currtime.tv_sec+(double)currtime.tv_usec/1000000.0)-starttime;
	if(thinktime-etime<.01) return-1;

	if (depth==0 || b.gameIsOver()) return b.getUtility(number);
	Board child=b;
	if(b.getTurn()=='1'){ //player 1 is max
		
		if(child.getNumLegalMoves()==0){
			child.pass();
			alpha=max(alpha,alphabeta(child,depth-1,alpha,beta,starttime));
			return alpha;
		}
		for(int r=0;r<8; r++){
			for(int c=0;c<8;c++){
				if(b.isLegal(r,c)){
					child=b;
					child.makeMove(r,c);
					alpha=max(alpha,alphabeta(child,depth-1,alpha,beta,starttime));
					if(beta<=alpha) break;	 
				}
			}
			if(beta<=alpha) break;	 //break out of both loops
		}
		return alpha;
	}else{
		if(child.getNumLegalMoves()==0){
			child.pass();
			beta=min(beta,alphabeta(child,depth-1,alpha,beta,starttime));
			return beta;
		}
		for(int r=0;r<8; r++){
			for(int c=0;c<8;c++){
				if(b.isLegal(r,c)){
					child=b;
					child.makeMove(r,c);
					beta=min(beta,alphabeta(child,depth-1,alpha,beta,starttime));
					if(beta<=alpha) break;	 
				}
				
			}
			if(beta<=alpha) break;	//break out of entire child examination loop 
		}
		return beta;
	}
}

int max(int a,int b){
	return a>=b?a:b;
}

int min(int a,int b){
	return a<=b?a:b;
}

int Player::getBestMove(Board  &b,int depth,double starttime){
	int moveVals[8][8],numMoves;
	//initialize moveVals to number out of band
	for(int i=0;i<8;i++)
		for(int j=0;j<8;j++)
			moveVals[i][j]=100000;
	int alpha=-1000000,beta=1000000;
	Board child=b;
	if(b.getTurn()=='1'){ //max's move
		for(int r=0;r<8; r++){
			for(int c=0;c<8;c++){
				if(b.isLegal(r,c)){
					child=b;
					child.makeMove(r,c);
					int alphatemp=alpha;
					alpha=alphabeta(child,depth-1,alpha-1,beta,starttime);
					if(alpha>alphatemp){//guaranteed to happen first time around
						moveVals[r][c]=alpha;
						numMoves=1;
					}else if(alpha==alphatemp){
						moveVals[r][c]=alpha;
						numMoves++;
					}else alpha=alphatemp;
				}
			}
		}
		struct timeval currtime;
		gettimeofday(&currtime,NULL);
		double etime=((double)currtime.tv_sec+(double)currtime.tv_usec/1000000.0)-starttime;
		if(thinktime-etime<.01) return -1;

		//choose best move randomly from candidates
		numMoves=rand()%numMoves +1; //random int in range [1-numMoves]
		int r,c;
		for(r=0;r<8&&numMoves>0;r++){
			for(c=0;c<8&&numMoves>0;c++)
				if(moveVals[r][c]==alpha) numMoves--;
		}
		r--;c--;
		return 10*r+c;
	}else{ //min's move
		for(int r=0;r<8; r++){
			for(int c=0;c<8;c++){
				if(b.isLegal(r,c)){
					child=b;
					child.makeMove(r,c);
					int betatemp=beta;
					beta=alphabeta(child,depth-1,alpha,beta+1,starttime);
					if(beta<betatemp){
						moveVals[r][c]=beta;
						numMoves=1;
					}else if(beta==betatemp){
						moveVals[r][c]=beta;
						numMoves++;
					}else beta=betatemp;
				}
			}
		}
		struct timeval currtime;
		gettimeofday(&currtime,NULL);
		double etime=((double)currtime.tv_sec+(double)currtime.tv_usec/1000000.0)-starttime;
		if(thinktime-etime<.01) return -1;

		//choose best move randomly from candidates
		numMoves=rand()%numMoves +1;  //random int in range [1-numMoves]
		int r,c;
		for(r=0;r<8&&numMoves>0;r++){
			for(c=0;c<8&&numMoves>0;c++)
				if(moveVals[r][c]==beta) numMoves--;
		}
		r--;c--;
		return 10*r+c;
	}
}

