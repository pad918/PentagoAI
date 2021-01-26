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
//670  lines 2020-09-19
//865  lines 2020-09-26
//1284 lines 2021-01-13

int main() {
	/*
	NNTrainer tester;
	tester.trainAgainstMinmax();
	*/
	
	
	srand(0);
	GameHandler game;
	game.playGame(0, 2);
	
	
	//stop program from stopping
	int c; std::cin >> c;
	return 0;
}