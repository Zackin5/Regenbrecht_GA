///////////////////////////////////////////
// Genetic Algorithms Assignment 
// GMNG 4322
// By Jace Regenbrecht
// Built in Visual Studio 2013 using C++11
///////////////////////////////////////////

#include <iostream>
#include <vector>
#include <ctime>
#include "GA.h"

using namespace std;

void main()
{
	int popsize = 200;		// Size of our population
	int ngens = 500;		// Number of generations
	int nsess = 200;		// Number of sessions
	int maxacts = 200;		// Max robot actions
	double mutprob = 0.005;	// Mutation probability
	bool crossover = true;	// Do we want crossover?

	// Gameworld size
	int worldx = 10;
	int worldy = 10;

	// Robby bot spawn position
	int robbystartx = 0;
	int robbystarty = 0;

	// Construct our genetic algorithm object with our parameters
	GA Alg1(popsize, ngens, nsess, mutprob, worldx, worldy, robbystartx, robbystarty, maxacts);

	// Run the algorithm
	Alg1.work(crossover);

}

