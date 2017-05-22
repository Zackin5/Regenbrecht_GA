#pragma once
#include "World.h"

using namespace std;

class Bot
{
public:
	int rewardLoot = 10;		// Loot reward value
	int penalCrash = 5;			// Crash penalty value
	int penalFalseLoot = 1;		// False loot penalty value
	int score = 0;				// Starting score
	int posx, posy;				// Integers to store the bot's current position
	World* gameworld = NULL;	// Pointer to reference the bot's world

	// Constructors
	Bot();
	Bot(int x, int y);
	Bot(int x, int y, World* world);
	Bot(World* world);

	// World setters and getters
	void setWorld(World* world);
	World* getWorld();

	void move(int move_x, int move_y);	// Movement function
	void loot();						// Loot function
	int peek(int x, int y);				// Peek function, returns '2' for a wall, '1' for loot or '0' for empty
	string getState();					// Function to get the robot's current state table
	void render();						// Function to draw the robot's location and the gameworld
};