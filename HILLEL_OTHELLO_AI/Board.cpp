
#include "Board.h"
#include "vt100.h"

Board::Board(){
	for (int i = 0; i < 8; ++i){
		for(int j=0; j<8;++j){
			board[i][j]='0';
		}
	}
	board[3][3]='2';board[3][4]='1';
	board[4][3]='1';board[4][4]='2';
	turn='1';
	updateLegalMoves();
	gameOver=false;
}

Board::Board(ifstream &input){
	input>>turn;
	char c;
	int a;
	for (int i = 0; i < 8; ++i){
		for(int j=0; j<8;++j){
			input>>c;
			if( !(c=='0' || c=='1' || c=='2')){
				cerr<<"Error loading file. Format is incorrest."<<endl;
				exit(-1);
			}
			board[i][j]=c;
		}
	}
	updateLegalMoves();
	gameOver=false;
	input.close();
}

void Board::displayBoard(){
	cout<<"    0     1     2     3     4     5     6     7 "<<endl;
	cout<<" *-----------------------------------------------*"<<endl;
	for(int i=0;i<8;i++){
		cout<<" |     |     |     |     |     |     |     |     |"<<endl;
		printRow(i);
		cout<<" |     |     |     |     |     |     |     |     |"<<endl;
		cout<<" *-----------------------------------------------*"<<endl;
	}
	cout<<"                  Player"<<turn<<"'s move:"<<endl<<endl;
}

void Board::printRow(int i){
	char c;
	cout<<i<<"|";
	for(int j=0;j<8;j++){
		cout<<"  ";
		switch(c=board[i][j]){
			case '0':
				if (moveList[i][j]){
					cout<<set_colors( VT_YELLOW, VT_DEFAULT);
					cout<<"*";
					cout<<finalize;	
				}else{
					cout<<" ";
				}
				break;
			case '1':
				cout<<set_colors( VT_GREEN, VT_GREEN);
				cout<<' ';
				cout<<finalize;
				break;
			case '2':
				cout<<set_colors( VT_WHITE, VT_WHITE);
				cout<<' ';
				cout<<finalize;
				break;
		}
		cout<<("  |");
	}
	cout<<endl;
}

void Board::updateLegalMoves(){
	int movecount=0;
	for (int i=0;i<8;i++){
			for (int j=0;j<8;j++){
				if (board[i][j]!='0') {
					moveList[i][j]=false;
					continue;
				}
				if (checkHorz(i,j)){
					moveList[i][j]=true;
					movecount++;
					continue;
				} 
				if (checkVert(i,j)){
					moveList[i][j]=true;
					movecount++;
					continue;
				}
				if (checkDiags(i,j)){
					moveList[i][j]=true;
					movecount++;
					continue;
				}
				moveList[i][j]=false;
			}
	}
	numLegalMoves=movecount;
}

bool Board::checkHorz(int i,int j){
	char opponent= (turn=='1'?'2':'1');
	int c=j;
	if (--c>=0 && board[i][c]==opponent){//check left
		while(--c>=0){
			if(board[i][c]==turn) return true;
			if(board[i][c]=='0') break;
		}
	}
	c=j;
	if (++c<8 && board[i][c]==opponent){//check right
		while(++c<8){
			if(board[i][c]==turn) return true;
			if(board[i][c]=='0') break;
		}
	}
	return false;
}

bool Board::checkVert(int i,int j){
	char opponent= (turn=='1'?'2':'1');
	int r=i;
	if (--r>=0 && board[r][j]==opponent){//check up
		while(--r>=0){
			if(board[r][j]==turn) return true;
			if(board[r][j]=='0') break;
		}
	}
	r=i;
	if (++r<8 && board[r][j]==opponent){//check down
		while(++r<8){
			if(board[r][j]==turn) return true;
			if(board[r][j]=='0') break;
		}
	}
	return false;
}

bool Board::checkDiags(int i,int j){
	char opponent= (turn=='1'?'2':'1');
	int r=i,c=j;
	if (--r>=0 && --c>=0 && board[r][c]==opponent){//check NW diag
		while(--r>=0 && --c>=0){
			if(board[r][c]==turn) return true;
			if(board[r][c]=='0') break;
		}
	}
	r=i,c=j;
	if (--r>=0 && ++c<8 && board[r][c]==opponent){//check NE diag
		while(--r>=0 && ++c<8){
			if(board[r][c]==turn) return true;
			if(board[r][c]=='0') break;
		}
	}
	r=i,c=j;
	if (++r<8 && --c>=0 && board[r][c]==opponent){//check SW diag
		while(++r<8 && --c>=0){
			if(board[r][c]==turn) return true;
			if(board[r][c]=='0') break;
		}
	}
	r=i,c=j;
	if (++r<8 && ++c<8 && board[r][c]==opponent){//check SE diag
		while(++r<8 && ++c<8){
			if(board[r][c]==turn) return true;
			if(board[r][c]=='0') break;
		}
	}
	return false;
}

//returns true is move was legal and false if not
bool Board::makeMove(int i,int j){
	if (!isLegal(i,j))	return false;
	
	char opponent= (turn=='1'?'2':'1');
	int c=j,cmark=-1;
	//Horizontal
	if (--c>=0 && board[i][c]==opponent){//check left
		while(--c>=0){
			if(board[i][c]==turn) {
				cmark=c;
				break;
			}
			if(board[i][c]=='0') break;
		}
		if (cmark!=-1){
			for(c=j;c>cmark;c--)
				board[i][c]=turn;
		}
	}
	c=j; cmark=-1;
	if (++c<8 && board[i][c]==opponent){//check right
		while(++c<8){
			if(board[i][c]==turn){
				cmark=c;
				break;
			} 
			if(board[i][c]=='0') break;
		}
		if (cmark!=-1)
			for(c=j;c<cmark;c++)
				board[i][c]=turn;
	}
	//Vertical
	int r=i,rmark=-1;
	if (--r>=0 && board[r][j]==opponent){//check up
		while(--r>=0){
			if(board[r][j]==turn) {
				rmark=r;
				break;
			}
			if(board[r][j]=='0') break;
		}
		if(rmark!=-1)
			for(r=i;r>rmark;r--)
				board[r][j]=turn;
	}
	r=i;rmark=-1;
	if (++r<8 && board[r][j]==opponent){//check down
		while(++r<8){
			if(board[r][j]==turn){
				rmark=r;
				break;
			} 
			if(board[r][j]=='0') break;
		}
		if (rmark!=-1)
			for(r=i;r<rmark;r++)
				board[r][j]=turn;
	}
	//Diagonal
	r=i,c=j,rmark=-1;
	if (--r>=0 && --c>=0 && board[r][c]==opponent){//check NW diag
		while(--r>=0 && --c>=0){
			if(board[r][c]==turn){
				rmark=r;
				break;
			} 
			if(board[r][c]=='0') break;
		}
		if(rmark!=-1)
			for(r=i,c=j;r>rmark;r--,c--)
				board[r][c]=turn;
	}
	r=i,c=j,rmark=-1;
	if (--r>=0 && ++c<8 && board[r][c]==opponent){//check NE diag
		while(--r>=0 && ++c<8){
			if(board[r][c]==turn){
				rmark=r;
				break;
			} 
			if(board[r][c]=='0') break;
		}
		if(rmark!=-1)
			for(r=i,c=j;r>rmark;r--,c++)
				board[r][c]=turn;
	}
	r=i,c=j,rmark=-1;
	if (++r<8 && --c>=0 && board[r][c]==opponent){//check SW diag
		while(++r<8 && --c>=0){
			if(board[r][c]==turn){
				rmark=r;
				break;
			} 
			if(board[r][c]=='0') break;
		}
		if(rmark!=-1)
			for(r=i,c=j;r<rmark;r++,c--)
				board[r][c]=turn;
	}
	r=i,c=j,rmark=-1;
	if (++r<8 && ++c<8 && board[r][c]==opponent){//check SE diag
		while(++r<8 && ++c<8){
			if(board[r][c]==turn){
				rmark=r;
				break;
			} 
			if(board[r][c]=='0') break;
		}
		if(rmark!=-1)
			for(r=i,c=j;r<rmark;r++,c++)
				board[r][c]=turn;
	}
	turn=opponent;
	updateLegalMoves();
	return true;
}

//called if no legal moves exist
void Board::pass(bool printMessage){
	char opponent= (turn=='1'?'2':'1');
	turn=opponent;
	updateLegalMoves();
	if(numLegalMoves==0){
		gameOver=true;
	} else if(printMessage) cout<<"Player"<<(turn=='1'?'2':'1')<<" must pass."<<endl<<endl;
}

int Board::getUtility(char player){
	int utility=0;
	switch(player){
		case '1':
		case '2':
			if(!gameOver){
				//pieces count
				double B=0,W=0;
				for(int r=0;r<8;r++){
					for(int c=0;c<8;c++){
						if(board[r][c]=='1'){
							B++;
						}else if(board[r][c]=='2')
							W++;
					}
				}
				int p=100*(B-W)/(B+W);
				//position weighted piece count
				int d=0;
				for(int r=0;r<8;r++){
					for(int c=0;c<8;c++){
						if(board[r][c]=='1'){
							d+=posvals[r][c];
						}else if(board[r][c]=='2')
							d-=posvals[r][c];
					}
				}
				//corners
				B=0;W=0;
				if(board[0][0]=='1') B++;
				if(board[0][7]=='1') B++;
				if(board[7][0]=='1') B++;
				if(board[7][7]=='1') B++;

				if(board[0][0]=='2') W++;
				if(board[0][7]=='2') W++;
				if(board[7][0]=='2') W++;
				if(board[7][7]=='2') W++;

				int c= 25*B-25*W;
				//Corner closeness 
				int l=0;
                if(board[0][0]=='0'){ 
                    if(board[1][1]=='1') l-=13; else if(board[1][1]=='2') l+=13;  
                    if(board[1][0]=='1') l-=6; else if(board[1][0]=='2') l+=6;  
                    if(board[0][1]=='1') l-=6; else if(board[0][1]=='2') l+=6; 
                } 
                if(board[0][7]=='0'){                
                    if(board[1][6]=='1') l-=13; else if(board[1][6]=='2') l+=13;  
                    if(board[1][7]=='1') l-=6; else if(board[1][7]=='2') l+=6;  
                    if(board[0][6]=='1') l-=6; else if(board[0][6]=='2') l+=6; 
                } 
                if(board[7][0]=='0'){    
                    if(board[6][1]=='1') l-=13; else if(board[6][1]=='2') l+=13;  
                    if(board[7][1]=='1') l-=6; else if(board[7][1]=='2') l+=6;  
                    if(board[6][0]=='1') l-=6; else if(board[6][0]=='2') l+=6; 
                } 
                if(board[7][7]=='0'){ 
                    if(board[6][6]=='1') l-=13; else if(board[6][6]=='2') l+=13; 
                    if(board[7][6]=='1') l-=6; else if(board[7][6]=='2') l+=6;  
                    if(board[6][7]=='1') l-=6; else if(board[6][7]=='2') l+=6; 
                }    
				//mobility
				int sign=turn=='1'?1:-1;
				B=0;W=0;
				B=numLegalMoves;
				flipturn();updateLegalMoves();
				W=numLegalMoves;
				int m=0;
				if(B!=W) m=sign*100*(B-W)/(B+W);
				//potential mobility
				B=0;W=0;
				for(int r=0;r<8;r++){
					for(int c=0;c<8;c++){
						if(board[r][c]=='1'){
						//	cout<<"player1: "<<r<<" "<<c<<utility<<endl;
							if(r-1>0 && board[r-1][c]=='0') B++;
							else if(r+1<8 && board[r+1][c]=='0') B++;
							else if(c-1>=0 && board[r][c-1]=='0') B++;
							else if(c+1<8 && board[r][c+1]=='0') B++;
							else if(r-1>=0 && c-1>=0 && board[r-1][c-1]=='0') B++;
							else if(r-1>=0 && c+1<8 && board[r-1][c+1]=='0') B++;
							else if(r+1<8 && c-1>=0 && board[r+1][c-1]=='0') B++;
							else if(r+1<8 && c+1<8 && board[r+1][c+1]=='0') B++;
						}else if(board[r][c]=='2'){
							//cout<<"player2: "<<r<<" "<<c<<utility<<endl;
							if(r-1>0 && board[r-1][c]=='0') W++;
							else if(r+1<8 && board[r+1][c]=='0') W++;
							else if(c-1>=0 && board[r][c-1]=='0') W++;
							else if(c+1<8 && board[r][c+1]=='0') W++;
							else if(r-1>=0 && c-1>=0 && board[r-1][c-1]=='0') W++;
							else if(r-1>=0 && c+1<8 && board[r-1][c+1]=='0') W++;
							else if(r+1<8 && c-1>=0 && board[r+1][c-1]=='0') W++;
							else if(r+1<8 && c+1<8 && board[r+1][c+1]=='0') W++;
						}
					}
				}
				int f=0;
				if(B!=0 && W!=0) f=100*(B-W)/(B+W);
				utility=2*p+20*d+120*c+45*f+60*m+80*l;
			}else{
				//pieces count
				for(int r=0;r<8;r++){
					for(int c=0;c<8;c++){
						if(board[r][c]=='1'){
							utility++;
						}else if(board[r][c]=='2')
							utility--;
					}
				}
				utility*=1000;
			}
			break;
	}
	return utility;
}

int Board::getScore(char player) const{
	int score=0;
	for(int r=0;r<8;r++){
		for(int c=0;c<8;c++){
			if(board[r][c]==player) score++;
		}
	}
	return score;
}

int Board::posvals[8][8]={
							  {50,-1,5,2,2,5,-1,50},
							  {-1,-10,1,1,1,1,-10,-1},
							  {5,1,1,1,1,1,1,5},
							  {2,1,1,0,0,1,1,2},
							  {2,1,1,0,0,1,1,2},
							  {5,1,1,1,1,1,1,5},
							  {-1,-10,1,1,1,1,-10,-1},
							  {50,-1,5,2,2,5,-1,50}
							};