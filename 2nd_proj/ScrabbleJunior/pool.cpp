#include "pool.h"

pool::pool()
{
	
}

void pool::addTile(char tile)
{
	poolTiles.push_back(tile);
}

char pool::getTile()
{
	int randomIndex = rand() % poolTiles.size();
	char tile = poolTiles[randomIndex];
	poolTiles.erase(poolTiles.begin() + randomIndex);
	return tile;
}

bool pool::isEmpty()
{
	return (poolTiles.size() == 0);
}

void pool::removeDuplicate(char letter)
{
	for (int i = 0; i < poolTiles.size(); i++)
	{
		if (poolTiles[i] == letter)
		{
			poolTiles.erase(poolTiles.begin() + i);
			break;
		}
	}
}

int pool::getSize() const
{
	return poolTiles.size();
}
