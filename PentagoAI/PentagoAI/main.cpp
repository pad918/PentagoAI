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
	NNTrainer tester;
	tester.testBackprop();
	
	/*
	srand(0);
	ptg::PentagoGame pentagoBoard;
	GameHandler game;
	mm::Minimax ai; ai.maxDepth = 4; ai.testVal = 0;
	game.playAgainstMinimax(pentagoBoard, 4);
	*/
	
	int c; std::cin >> c;
	return 0;
}