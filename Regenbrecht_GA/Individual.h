#pragma once
#include <vector>

using namespace std;

// Structure to store each individual's chromosone and fitness score
struct individual
{
	//vector<short int> chromosone;
	short int chromosone[243] = {};
	float score;
	friend bool operator ==(individual i, individual j) { return (i.score == j.score); }
	friend bool operator !=(individual i, individual j) { return (i.score != j.score); }
	friend bool operator <(individual i, individual j) { return (i.score < j.score); }
	friend bool operator >(individual i, individual j) { return (i.score > j.score); }
};