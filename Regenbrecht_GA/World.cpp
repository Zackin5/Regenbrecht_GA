#include "World.h"

World::World()
{
	World(10, 10);
}

World::World(int x, int y)
{
	sizex = x;
	sizey = y;
	lootGrid.resize(sizex, vector<bool>(sizey, false));
}

void World::generateLoot()
{
	// Using C++11's random libary, stdlib's rand wasn't random enough in generating the loot for multiple generations.
	random_device randn;

	// Loop through each world tile and roll if it should contain loot or not
	for (int x = 0; x < sizex; x++)
		for (int y = 0; y < sizey; y++)
			setLoot(x, y, randn() % 2);
}

bool World::setLoot(int x, int y, bool value)
{
	if (x < sizex && y < sizey)
	{
		lootGrid[x][y] = value;
		return true;
	}
	else
		return false;
}

bool World::getLoot(int x, int y)
{
	return lootGrid[x][y];
}

void World::render()
{
	for (int x = 0; x < sizex; x++)
	{
		for (int y = 0; y < sizey; y++)
			cout << lootGrid[x][y];
		cout << endl;
	}
	cout << endl;
}