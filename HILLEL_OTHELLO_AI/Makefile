othello.exe: othello.o Player.o Board.o 
	g++ -o othello.exe othello.o Player.o Board.o

othello.o: othello.cpp
	g++ -c othello.cpp

Player.o: Player.cpp Player.h
	g++ -c Player.cpp

Board.o: Board.cpp Board.h
	g++ -c Board.cpp

debug:
	g++ -g -o othelloDebug.exe othello.cpp Player.cpp Board.cpp

clean:
	rm -f *.exe *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp Makefile backups
