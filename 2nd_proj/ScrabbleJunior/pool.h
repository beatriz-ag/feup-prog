#ifndef POOL_H
#define POOL_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <time.h>
using namespace std;

class pool
{
public:
	pool(); 
	void addTile(char tile);
	char getTile();                  // returns a letter from the pool
	bool isEmpty();
	void removeDuplicate(char letter);
	int getSize() const;

private:
	vector<char> poolTiles;
};

#endif 
