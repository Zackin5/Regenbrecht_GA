#include "Bot.h"

Bot::Bot()
{
	Bot(0, 0);
}

Bot::Bot(int x, int y)
{
	posx = x;
	posy = y;
}

Bot::Bot(int x, int y, World* overworld)
{
	posx = x;
	posy = y;
	setWorld(overworld);
}

Bot::Bot(World* overworld)
{
	posx = 0;
	posy = 0;
	setWorld(overworld);
}


void Bot::setWorld(World* overworld)
{
	gameworld = overworld;
}

World* Bot::getWorld()
{
	return gameworld;
}

void Bot::move(int x, int y)
{
	// Check if our move would go out of bounds
	if (posx + x >= gameworld->sizex || posx + x < 0 || posy + y >= gameworld->sizey || posy + y < 0)
		// Penalize if it would
		score -= penalCrash;	
	else
	{	// If not, then do the move
		posx += x;
		posy += y;
	}
}

void Bot::loot()
{
	// If there's loot, give a score bonus and remove the loot
	if (gameworld->getLoot(posx, posy))
	{
		score += rewardLoot;
		gameworld->setLoot(posx, posy, false);
	}
	else
		// Else, penalize for the effort
		score -= penalFalseLoot;
}

int Bot::peek(int x, int y)
{
	// If the space is out of bounds, return 2 for a wall state
	if (posx + x >= gameworld->sizex || posx + x < 0 || posy + y >= gameworld->sizey || posy + y < 0)
		return 2;
	else
		// Otherwise return the loot state, either 0 for no loot or 1 for loot
		return gameworld->getLoot(posx + x, posy + y);
}

string Bot::getState()
{
	string rtnstr = "";		// Return string

	// Peek at each slot next to our bot and at it's current position, then push the results to our state string
	rtnstr.push_back('0' + peek(0, 1));
	rtnstr.push_back('0' + peek(0, -1));
	rtnstr.push_back('0' + peek(-1, 0));
	rtnstr.push_back('0' + peek(1, 0));
	rtnstr.push_back('0' + peek(0, 0));

	// Return the state string
	return rtnstr;
}

void Bot::render()
{
	for (int i = 0; i < gameworld->sizex; i++)
	{
		for (int j = 0; j < gameworld->sizey; j++)
			if (i == posx && j == posy)
				cout << 'R';
			else
				if (gameworld->getLoot(i, j))
					cout << "x";
				else
					cout << " ";
		cout << endl;
	}
	cout << endl;
}