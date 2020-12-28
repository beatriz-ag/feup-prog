#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <iostream>
using namespace std;

class player
{
public:
	player();
	void addPoints(int p);
	int playerPoints() const;
	void addTile(char tile);
	void removeTile(char tile);
	char removeTile();
	bool checkTile(char tile);
	void showHand() const;
	int handSize() const;
	void recordPossiblePlay(char x, char y);
	void showHints() const;
	int hintsAvailable() const;
	void showAvailableHints() const;
	void decreaseHints();

private:
	int points;
	vector<char> hand;
	int hints;
	pair<char, char> lastPossiblePlay;
};

#endif 