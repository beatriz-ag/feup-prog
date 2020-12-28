#include "player.h"

player::player()
{
	points = 0;
	hints = 3;
}

void player::addPoints(int p)
{
	points += p;
}

int player::playerPoints()const
{
	return points;
}

void player::addTile(char tile)
{
	hand.push_back(tile);
}

void player::removeTile(char tile)
{
	for (int i = 0; i < hand.size(); i++)
	{
		if (hand[i] == tile)
		{
			hand.erase(hand.begin() + i);
			return;
		}
	}
}

char player::removeTile()
{
	char tile = hand[hand.size()-1];
	hand.pop_back();
	return tile;
}

bool player::checkTile(char tile)
{
	for (int i = 0; i < hand.size(); i++)
	{
		if (hand[i] == tile)
		{
			return true;
		}
	}
	return false;
}

void player::showHand()const
{
	cout << "Letters you own: ";
	for (int i = 0; i < hand.size(); i++)
	{
		cout << hand[i] << " ";
	}
	cout << endl;
}

int player::handSize() const
{
	return hand.size();
}

void player::recordPossiblePlay(char x, char y)
{
	lastPossiblePlay.first = x;
	lastPossiblePlay.second = y;
}

void player::showHints() const
{
	cout << "There is a possible move in position " << lastPossiblePlay.first << lastPossiblePlay.second<<endl;
}

int player::hintsAvailable() const
{
	return hints;
}

void player::showAvailableHints() const
{
	cout << "Hints: " << hints<<endl<<endl;
}

void player::decreaseHints() 
{
	hints--;
}
