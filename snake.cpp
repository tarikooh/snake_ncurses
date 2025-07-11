#include <ncurses.h>
#include <cstdlib>
#include <list>
#include <ctime>

using namespace std;

enum Dir {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

int main(int argc, char* argv[]){
	initscr();
	srand(time(0));
	int max_x, max_y;
	getmaxyx(stdscr, max_y, max_x);

	int ball_x = 7, ball_y = 5;
	list<int> x = {1,2,3,4,5};
	list<int> y = {1,1,1,1,1};

	int head_x = 5, head_y = 1;
	int prev_x = 3, prev_y = 1; 

	bool game_over = false;
	Dir dir = RIGHT;
	int ch = 0;
  
	WINDOW* win = newwin(max_y/2, max_x/2, max_y/4, max_x/4);
	refresh();
	keypad(stdscr, true);
	timeout(300);

	getmaxyx(win, max_y, max_x);
	int score = 0;
	int col = -1;

	while(!game_over){
		switch(ch){
		case KEY_UP:
			if(dir != DOWN)
				dir = UP;
			break;
		case KEY_DOWN:
			if(dir != UP)
				dir = DOWN;
			break;
		case KEY_LEFT:
			if(dir != RIGHT)
				dir = LEFT;
			break;
		case KEY_RIGHT:
			if(dir != LEFT)
				dir = RIGHT;
			break;
		default:
			break;
		}
		
		prev_x = head_x;
		prev_y = head_y;
		switch(dir){
		case LEFT:
			head_x = prev_x - 1;
			head_y = prev_y;
			break;
		case RIGHT:
			head_x = prev_x + 1;
			head_y = prev_y;
			break;
		case DOWN:
			head_y = prev_y + 1;
			head_x = prev_x;
			break;
		case UP:
			head_y = prev_y - 1;
			head_x = prev_x;
			break;
		}
		x.push_back(head_x);
		y.push_back(head_y);
		y.pop_front();
		x.pop_front();

		wclear(win);
		box(win, 0, 0);
		mvwprintw(win, ball_y, ball_x, "O");
		mvwprintw(win, max_y-1, 10, "Score: %d", score);
    

		col = -1;
		for(int i=0; i<x.size(); i++){
			mvwprintw(win, *next(y.begin(), i), *next(x.begin(), i), "X");
			if(*next(y.begin(), i) == y.back() && *next(x.begin(), i) == x.back()) col++;
		}
		if(y.back() == max_y - 1 || y.back() == 0 || x.back() == max_x -1 || x.back() == 0) col = 1;
		mvprintw(0, 0, " "); // don't want the cursor to clutter the game board
    
		if(col>0) game_over = true;

		if(*next(y.end(), -1) == ball_y && *next(x.end(), -1) == ball_x){
			score++;
			x.push_front(*next(x.begin(), 0));
			y.push_front(*next(y.begin(), 0));
			ball_y = rand() % (max_y-2) + 1;
			ball_x = rand() % (max_x-2) + 1;
		}
		
		wrefresh(win);
		ch = getch();
	}

	getmaxyx(stdscr, max_y, max_x);
	WINDOW* game_over_win = newwin(5, 15, max_y/2-5, max_x/2-10);
	refresh();
	box(game_over_win, 0, 0);
	mvwprintw(game_over_win, 2, 3, "Game over");
	mvwprintw(game_over_win, 3, 3, "Scored: %d", score);
	wrefresh(game_over_win);

	timeout(-1);
	getch();
	endwin();

	return 0;
}
