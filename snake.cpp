#include <ncurses.h>
#include <vector>
#include <cstdlib>
using namespace std;

void drawBoard(int xLim, int yLim);

int main( int arg, char ** argv ) {
	vector<int> tailX = {10, 9, 8, 7, 6}, tailY = {10, 10, 10, 10, 10}, xTmp, yTmp;
	int xDir = 1, yDir = 0;
	int xLim = 60, yLim = 30; 
	int xNug = rand() % xLim, yNug = rand() % yLim;	// 'nugget', makes snake longer 
	char plyr = '#', nugget = 'o';
	bool isGameOver = false;
	
	initscr();
	cbreak();		// getch() without Enter
	nodelay(stdscr, TRUE);	// doesn't wait for kbd entry
	noecho();		// stops keypresses showing at cursor
	curs_set(FALSE);	// disables cursor

	drawBoard(xLim, yLim);

	// game loop
	while(!isGameOver) {
		for(int i=0; i<tailX.size(); i++) {
			mvaddch(tailY[i], tailX[i], plyr);
		}
		mvaddch(yNug, xNug, nugget);
		refresh();
		switch(getch()) {
			case 'h' : if(xDir == 1) { break; } xDir = -1; yDir = 0; break;
			case 'j' : if(yDir == -1) { break; } xDir = 0; yDir = 1; break;
			case 'k' : if(yDir == 1) { break; } xDir = 0; yDir = -1; break;
			case 'l' : if(xDir == -1) { break; } xDir = 1; yDir = 0; break;
		}
		// make tail follow head
		xTmp = tailX; yTmp = tailY;
		mvaddch(yTmp[yTmp.size()-1], xTmp[xTmp.size()-1], ' '); // delch() messes with alignment
		tailX[0] += xDir; tailY[0] += yDir;
		for(int i=1; i<tailX.size(); i++){
			tailX[i] = xTmp[i-1];
			tailY[i] = yTmp[i-1];
		}
		// snake length condition
		if( tailX[0] == xNug && tailY[0] == yNug ) {
			mvaddch(yNug, xNug, ' ');
			xNug = rand() % xLim; yNug = rand() % yLim; // add segment to tail, new nugget
			tailX.push_back(xTmp[xTmp.size()-1]); tailY.push_back(yTmp[yTmp.size()-1]);
		}
		// game over conditions
	        if(tailX[0] >= xLim || tailY[0] >= yLim || tailX[0] <= 0 || tailY[0] <= 0)
			isGameOver = true;
		for(int i=1; i<tailX.size(); i++) {
			if ( tailX[0] == tailX[i] && tailY[0] == tailY[i] )
				isGameOver = true; break;
		}

		napms(100);
	}
	mvprintw(yLim/2, (xLim/2)-4, "GAME OVER");
	refresh();
	napms(2000);
	endwin();

	return 0;
}

void drawBoard(int xLim, int yLim) {
	// corners
	mvaddch(0,0,'+'); mvaddch(0,xLim,'+'); mvaddch(yLim,0,'+'); mvaddch(yLim,xLim,'+');
	// sides
	for(int i=1; i<xLim; i++) {
		mvaddch(0,i,'-');
		mvaddch(yLim,i,'-');
	}
	for(int i=1; i<yLim; i++) {
		mvaddch(i,0,'|');
		mvaddch(i,xLim,'|');
	}
}


