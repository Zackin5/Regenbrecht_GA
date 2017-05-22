# Genetic Algorithm Project

## Project Overview

A simple genetic algorithm program in which a simulated robotic agent collects randomly placed loot in a world. Based on an assignment from the Artificial Intelligence course at the University of Houston - Victoria, which was in turn adapted from one at the Portland State University. Project features both mutation, crossover and roulette wheel selection in it's implementation. 

## Program Readme

Written in C++11 and built using Visual Studio 2013. During run-time a .csv file will be generated in the root directory in which fitness statistics for each generation will be saved for later reference. The algorithm parameters can be customized in Main.cpp while parameters for Bot rewards and penalties are changeable in Bot.h

Multithreading has been added to the project to provide a performance boost during runtime. The number of threads used can be set using the value of threadn at line 56 in GA.h.

## Project files and descirptions

Main.cpp contains the main function as well as most of the customizable parameters for the genetic algorithm. The GA class serves as the main codebase for the genetic algorithm execution and evaluation. World is a simple class for keeping track of the gameworld in which loot is placed and the Robby bot moves in, while Bot is the class for the Robby bot itself, containing some modifiable parameters for setting action reward and penalty values. The Individual structure is used to store the chromosome and score of a individual, and also offers comparison parameters.
