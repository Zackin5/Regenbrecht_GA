#pragma once
#include <vector>
#include <iostream>
#include <random>

using namespace std;

class World
{
private:
	vector<vector<bool>> lootGrid;	// 2D vector to store loot positioning
public:
	int sizex, sizey;				// Gameworld dimensions

	// Constructors
	World();
	World(int x, int y);

	void generateLoot();					// Loot generator function
	bool setLoot(int x, int y, bool value);	// Manual loot set function
	bool getLoot(int x, int y);				// Returns if a position has loot or not
	void render();							// Function to print the world and it's loot locations
};
