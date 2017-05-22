#pragma once
#include <vector>
#include <random>
#include <iostream>
#include <fstream>
#include <string>
#include <numeric>
#include <algorithm>
#include <ctime>
#include <thread>
#include <mutex>
#include <future>
#include "World.h"
#include "Bot.h"
#include "Individual.h"

using namespace std;

class GA
{
private:
	int popsize;				// Population size
	int numgenerations;			// Number of generations
	int numsessions;			// Number of sessions
	double mutationprob;		// Mutation probability
	int maxactions;				// Max number of bot actions
	const int statetableSize = 243;	// Constant to store our state table size, changing this will likely break things
	vector<string> statetable;	// Vector for storing the state table
	vector<individual> indivs;	// Vector of our individuals
	ofstream resultsfile;		// Result file object
	random_device randn;		// Random object

	// Our gameworld dimensions
	int worldx;
	int worldy;

	// Our bot start position
	int robbystartx;
	int robbystarty;

	void generateChromosones();		// Function to generate the chromosones
	void generateStateTable();		// Function to setup the table containing all 243 possible bot states
	void printChromosones();		// Function to print out current chromosones
	void printChromosonesHash();	// Function to print out hashes of all current chromosones
	void printFitness();			// Function to print out all current individuals' fitnesses
	int printMeanFitness();			// Function to print and return the overall mean of the current individuals' fitnesses
	int printBestFitness();			// Function to print and return the best fit individual
	int stateTableLookup(string search_string);																	// Function to return the state table index that matches the passed state
	void actionExec(int individual_index, int session_count);													// Function for fitness evaluation stage
	int sessionLoop(int session_start, int session_end, int individual_index);									// Loop for session evaluation
	vector<individual> roulettewheelselection(vector<individual> generation_individuals, int return_number);	// Roulette wheel selection, adapted from the labwork assignment
	vector<individual> crossover(vector<individual> generation_individuals);									// Crossover function
	vector<individual> mutate(vector<individual> generation_individuals);										// Mutation function

	// Multithreading
	unsigned short int threadn = 8;	// Number of threads to use. 8 seems about the cutoff for deminishing returns, but will likely vary depending on hardware
	mutex mtxPrint;					// Mutex to use for printing inorder to avoid jumbled text

public:
	GA();
	GA(int population_size, int number_of_generations, int number_of_sessions, double mutation_probability, int worldsize_x, int worldsize_y, int robbystart_x, int robbystart_y, int max_actions);

	void work(bool do_crossover);				// The main function to begin the GA
};