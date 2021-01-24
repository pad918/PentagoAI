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

class NNTrainer {
private:

public:
	void testBackprop();
	void trainAgainstMinmax();
};