#include <iostream>
#include <stdlib.h>
#include <chrono>
#include "Pentago.h"
#include "Minimax.h" 
#include "MyMath.h"
#include "PlayGame.h"
#include "Hash.h"
#include "NeuralNetwork.h"
#include "NNTrainer.h"
#include "Dataset.h"
#include "Benchmark.h"
//670  lines 2020-09-19
//865  lines 2020-09-26
//1284 lines 2021-01-13
//1745 lines 2021-02-01

int main() {
	
	//DEBUG:
	//benchmarkBoardEvaluation();

	//srand(time(NULL));
	NNTrainer tester;
	//tester.trainAgainstDataset();
	
	//tester.generateData(4);
	//tester.createTrainingData(0, 1);
	//tester.trainAgainstDataset();

	
	srand(0);
	GameHandler game;
	game.playGame(0, 2);
	
	
	//stop program from stopping
	int c; std::cin >> c;
	return 0;
}