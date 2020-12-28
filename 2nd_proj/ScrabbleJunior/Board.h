#ifndef BOARD_H
#define BOARD_H

#define white 15
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include "player.h"
#include "pool.h"
using namespace std;

struct tile {
	char letter = ' ';
	bool openPlay = false;
	bool empty = true;
	pair <bool, bool> horizvert = { false, false };
	bool played = false;
	int color = white;
	pair <bool, bool> firstLetterHV = { false,false };
	pair <bool, bool> endLetterHV = { false,false };
	pair <int, int> wordsize = { 0, 0 };
};

class Board
{
public:
	Board(ifstream& boardfile, int nrplayers);
	void clrscr();
	void setcolor(unsigned int color);
	void display();
	bool checkPlay(char letter, int posx, int posy);
	bool checkPossibilities(int indexplayer);
	void checkPoints(int x, int y, int indexplayer);
	void replaceTiles(int indexplayer, int xplay);
	void removeDuplicates();
	void swapTiles(int index);
	void openPlaysWord(int posx, int posy);
	bool cheating(char letter, int indexplayer);
	bool lookForOpenTiles();
	void play();
	void rules();
	void end();
	pair<char, char> Hint(int indexplayer);
		
private:
	size_t sizeCol, sizeLine;
	vector<vector <tile>> BoardVector;
	vector<player> players;
	int nrplayers;
	static int xplayer;
	pool Pool;
};

#endif 