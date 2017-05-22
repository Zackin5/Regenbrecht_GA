#include "GA.h"

GA::GA()
{
	GA(20, 50, 20, 0.005, 10, 10, 0, 0, 20);
}

GA::GA(int psize, int ngen, int nsess, double mutp, int worldsize_x, int worldsize_y, int robby_x, int robby_y, int maxacts)
{
	popsize = psize;
	numgenerations = ngen;
	numsessions = nsess;
	mutationprob = mutp;
	maxactions = maxacts;
	worldx = worldsize_x;
	worldy = worldsize_y;
	robbystartx = robby_x;
	robbystarty = robby_y;
	indivs.resize(popsize);
}

void GA::generateChromosones()
{
	//random_device randn;

	vector<short int> minichrome;

	for (int i = 0; i < popsize; i++)
	{
		for (int j = 0; j < statetableSize; j++)
			indivs[i].chromosone[j] = randn() % 7;
	}
}

void GA::generateStateTable()
{
	string pushstr = "";

	// Jesus christ it's a 5x nested for loop, all in the name of not doing manually written checks for each possible bot state
	// Generate each line of the state table
	for (int i1 = 0; i1 < 3; i1++)
		for (int i2 = 0; i2 < 3; i2++)
			for (int i3 = 0; i3 < 3; i3++)
				for (int i4 = 0; i4 < 3; i4++)
					for (int i5 = 0; i5 < 3; i5++)
					{
						pushstr = to_string(i1) + to_string(i2) + to_string(i3) + to_string(i4) + to_string(i5);
						statetable.push_back(pushstr);
					}
}

int GA::stateTableLookup(string srchstr)
{
	int index = 0;

	// Using maths to get the index of our passed state table
	index += (int)(srchstr[0] - '0') * 81;
	index += (int)(srchstr[1] - '0') * 27;
	index += (int)(srchstr[2] - '0') * 9;
	index += (int)(srchstr[3] - '0') * 3;
	index += (int)(srchstr[4] - '0');

	return index;
}

void GA::printChromosones()
{
	for (int i = 0; i < popsize; i++)
	{
		for (int j = 0; j < statetableSize; j++)
			cout << indivs[i].chromosone[j];
		cout << endl;
	}
}

void GA::printChromosonesHash()
{
	string curchromo = "";
	string hashedstr = "";
	hash<string> hashstr;

	// Nobody look at this nasty hackjob of a function please
	for (int i = 0; i < popsize; i++)
	{
		for (int j = 0; j < statetableSize; j++)
			curchromo.append(to_string(indivs[i].chromosone[j]));

		hashedstr.append(to_string(hashstr(curchromo)));
		hashedstr += '\n';
	}

	cout << hashedstr << endl;
}

void GA::printFitness()
{
	for (int i = 0; i < popsize; i++)
		if (indivs[i].score != 0)
			cout << "individual " << i << " fitness score: " << indivs[i].score << endl;
}

int GA::printBestFitness()
{
	int bestindiv = 0;

	for (int i = 0; i < indivs.size(); i++)
		if (indivs[i].score > indivs[bestindiv].score)
			bestindiv = i;

	cout << "Best fitness: " << indivs[bestindiv].score << endl;

	return indivs[bestindiv].score;
}

int GA::printMeanFitness()
{
	float sum = 0;

	for (int i = 0; i < popsize; i++)
		sum += indivs[i].score;

	sum /= popsize;

	cout << "Mean fitness: " << sum << endl;

	return sum;
}

int GA::sessionLoop(int pstart, int pend, int indv)
{
	//random_device randn;	// Random object
	int stateslot;			// State index
	int scoresum = 0;		// Final score sum

	World Trialworld;
	Bot Trialbot;

	for (int ss = pstart; ss <= pend; ss++)
	{
		// Regenerate our world
		Trialworld = World(worldx, worldy);
		Trialworld.generateLoot();

		// Refresh our bot
		Trialbot = Bot(robbystartx, robbystarty, &Trialworld);

		// Loop until we reach the max number of actions allowed
		for (int k = 0; k < maxactions; k++)
		{
			// Get our state index
			stateslot = stateTableLookup(Trialbot.getState());

			// Perform an action based on the chromosone's value for the bot's current state
			switch (indivs[indv].chromosone[stateslot])
			{
			case 0:
				// Move north
				Trialbot.move(0, 1);
				break;
			case 1:
				// Move south
				Trialbot.move(0, -1);
				break;
			case 2:
				// Move east
				Trialbot.move(-1, 0);
				break;
			case 3:
				// Move west
				Trialbot.move(1, 0);
				break;
			case 4:
				// Random movement case

				// Roll a random number between 0 and 3 (inclusive) and pick a movement direction based on that roll
				switch (randn() % 4)
				{
				case 0:
					Trialbot.move(0, 1);
					break;
				case 1:
					Trialbot.move(0, -1);
					break;
				case 2:
					Trialbot.move(-1, 0);
					break;
				case 3:
					Trialbot.move(1, 0);
					break;
				default:
					// Unknown case
					mtxPrint.lock();
					cout << "ERROR: Unknown random move in indv " << indv << " at state slot " << stateslot << endl;
					mtxPrint.unlock();
					break;
				}
				break;
			case 5:
				// Stay put case
				break;
			case 6:
				// Loot case
				Trialbot.loot();
				break;
			default:
				// Unknown case
				mtxPrint.lock();
				cout << "ERROR: Unknown case " << indivs[indv].chromosone[stateslot] << " from indv " << indv << " at state slot " << stateslot << endl;
				mtxPrint.unlock();
				break;
			}
		}

		// Add the new score to the sum
		scoresum += Trialbot.score;
	}

	// Return the final sum
	return scoresum;
}

vector<individual> GA::roulettewheelselection(vector<individual> pindiv, int nrtn)
{
	vector<float> probranges;		// Vector to store probabilty ranges
	vector<individual> rtnindivs;	// Vector to store returned indivuals
	float spinsum = 0.0;			// Sum of all previous random spins
	int popsum = 0;					// Factorial varible

	for (int n = 0; n <= popsize; n++)
		popsum += n;

	// Create the probability ranges
	for (int i = 0; i < pindiv.size(); i++)
	{
		float sum = 0;

		// Make each probability range the sum of it and it's previous values
		for (int j = 1; j <= i; j++)
			sum += (popsize - j + 1.0) / (float) popsum; // Adapted to use the individual's fitness rank vs actuall fitness score

		// Push each generated range to the probabilty range vector
		probranges.push_back(sum);
	}

	// Spin the wheel for the number of wanted individuals
	for (int i = 0; i < nrtn; i++)
	{
		int j = 0;	// Iterator

		spinsum += rand() % 101 / 100.0;	// Add a random value between 0.0 and 1.0 to our current spin

		if (spinsum > 1.0)
			spinsum -= 1.0;

		// Increment our iterator until we find a probability range element greater than our random value
		while (spinsum > probranges.at(j) && j < pindiv.size() - 1)
			j++;

		// Push the fit individual that we're in the probability range of
		rtnindivs.push_back(pindiv[j]);
	}

	return rtnindivs;
}

vector<individual> GA::crossover(vector<individual> pindivs)
{
	//random_device randn;			// Random object
	vector<individual> rtnindivs;	// Vector of individuals to return
	int cuti;						// Cut index

	rtnindivs.resize(2);				// Resize the return vector
	cuti = randn() % statetableSize;	// Pick a random index to cut at

	// First return individual
	// Copy the first individual's chromosomes up to the cut
	for (int j = 0; j < cuti; j++)
		rtnindivs[0].chromosone[j] = pindivs[0].chromosone[j];

	// Copy the second individual's chromosomes past the cut
	for (int k = cuti; k < statetableSize; k++)
		rtnindivs[0].chromosone[k] = pindivs[1].chromosone[k];

	// Second return individual
	// Copy the first individual's chromosomes up to the cut
	for (int j = 0; j < cuti; j++)
		rtnindivs[1].chromosone[j] = pindivs[1].chromosone[j];

	// Copy the second individual's chromosomes past the cut
	for (int k = cuti; k < statetableSize; k++)
		rtnindivs[1].chromosone[k] = pindivs[0].chromosone[k];

	// Reset the new individuals' scores
	rtnindivs[0].score = 0;
	rtnindivs[1].score = 0;

	// Return the individuals
	return rtnindivs;
}

vector<individual> GA::mutate(vector<individual> pindivs)
{
	//random_device randn;	// Random object
	double mutchance;		// For storing our mutation chance roll

	for (int currenti = 0; currenti < pindivs.size(); currenti++)		// Loop through each individual
		for (int j = 0; j < statetableSize; j++)	// Loop through each chromosome slot of the individual
		{
			// Roll the probability of mutation
			mutchance = (randn() % 1000) / 1000.0;

			// If the rolled mutation chance is less than the mutation probability value mutate that element
			if ( mutchance < mutationprob)			
				pindivs[currenti].chromosone[j] = randn() % 7;
		}

	// Return our newly mutated individuals
	return pindivs;
}


void GA::work(bool bcrossover)
{
	float percentage = 0;	// Local variable to store our percentage of individual evaluations

	// Psudo benchmarking
	time_t starttime;
	time_t endtime;

	// Prep our results file
	resultsfile.open("results_" + to_string(time(0)) + ".csv");
	resultsfile << "Population Size, Generations, Sessions, Actions per Session, Mutation Probability, Crossover Enabled, Thread Count\n";
	resultsfile << popsize << ',' << numgenerations << ',' << numsessions << ',' << maxactions << ',' << mutationprob << ',' << bcrossover << ',' << threadn << '\n';

	resultsfile << "Generation,Top Fitness,Mean Fitness\n";

	// Save our start time
	time(&starttime);

	// Generate our chromosomes and the state table we'll be referencing
	generateChromosones();
	generateStateTable();

	// Generation loop
	for (int i = 0; i < numgenerations; i++)
	{
		vector<individual> newpop;		// Vector to store our new population in
		vector<individual> rtnindivs;	// Vector to temp store individuals created during crossover

		// Print our current generation
		cout << "GENERATION " << i;
		resultsfile << i << ',';

		// Individual loop
		for (int indv = 0; indv < popsize; indv++)
		{
			vector<future<int>> futureSums;	// Vector to store our threads' future score sum returns
			int scoresum = 0;				// Final score sum int

			// Start our first thread and save the reference to the future return values
			futureSums.push_back(async(&GA::sessionLoop, this, 0, numsessions / threadn + numsessions % threadn, indv));

			// Start our next set of threads
			for (int thr = 1; thr < threadn; thr++)
				futureSums.push_back(async(&GA::sessionLoop, this, (numsessions / threadn) * thr + 1, (numsessions / threadn) * (thr + 1), indv));

			// Fetch all of the return values on complete and sum all of the returned score sums
			while (!futureSums.empty())
			{
				scoresum += futureSums.back().get();
				futureSums.pop_back();
			}

			// Calculate the final individual score
			indivs[indv].score = scoresum / (float)numsessions;

			// Print a simple progress bar
			if (indv / (float)popsize > percentage)
			{
				percentage += 0.0625;
				cout << '.';
			}
		}

		percentage = 0;
		cout << endl;

		// Print the fitness results and save them to the result file
		resultsfile << printBestFitness() << ',';
		resultsfile << printMeanFitness() << ',';
		resultsfile << '\n';
		cout << endl;

		// Sort by fitness
		sort(indivs.rbegin(), indivs.rend());

		// Populate the new population using crossover (if enabled)
		do
		{
			// Is crossover enabled?
			if (bcrossover)
				// If it is, preform crossovers on two individuals returned from the roulette section and save them
				rtnindivs = crossover(roulettewheelselection(indivs, 2));
			else
				// If not, just save our roulette wheel's selections
				rtnindivs = roulettewheelselection(indivs, 2);

			// Push our returned individuals to our new population
			for (int i = 0; i < rtnindivs.size(); i++)
				newpop.push_back(rtnindivs[i]);
		} while (newpop.size() != indivs.size());

		// Mutate the individuals and save them as our new population
		indivs = mutate(newpop);
	}


	// We're done here
	time(&endtime);
	cout << "GA complete, results saved to .\\results_time().csv" << endl;
	cout << "Time: " << starttime << " -- " << endtime << " == " << difftime(endtime, starttime) << endl;
	resultsfile << starttime << ',' << endtime << ',' << difftime(endtime, starttime) << '\n';

	resultsfile.close();
}