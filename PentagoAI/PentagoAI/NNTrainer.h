#pragma once
#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <Eigen/Dense>
#include "Pentago.h"
#include "Minimax.h" 
#include "MyMath.h"
#include "PlayGame.h"
#include "Hash.h"
#include "NeuralNetwork.h"
#include "Dataset.h"

class NNTrainer {
private:
	double getLearningRate(int epoch);
public:
	void testBackprop();
	void generateData(int numThreads);
	void createTrainingData(int offset, int stepSize);
	void trainAgainstDataset();
	void trainAgainstMinmax();
};