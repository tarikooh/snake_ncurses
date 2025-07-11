all: snake

snake: snake.cpp
	g++ snake.cpp -o snake -lncurses
