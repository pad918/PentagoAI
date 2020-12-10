#include <iostream>
#include <stdlib.h>
#include <chrono>
#include "Pentago.h"
#include "Minimax.h" 
#include "MyMath.h"
#include "PlayGame.h"
#include "Hash.h"
#include "NeuralNetwork.h"
//670 lines 2020-09-19
//865 lines 2020-09-26

int main() {
	srand(time(NULL));
	std::vector<int> layerSizes; 
	layerSizes.push_back(2);
	layerSizes.push_back(10);
	layerSizes.push_back(2);
	NeuralNetwork nn(layerSizes);
	//nn.loadNetwork("smallTest.txt");
	//nn.saveNetwork("nameless.txt");
	Eigen::MatrixXd inputs(2, 1);
	inputs(0, 0) = 0.0f;
	inputs(1, 0) = 0.33f;
	nn.setInputs(inputs);
	nn.calculateOutputs();

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