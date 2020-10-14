#pragma once
#include <Eigen/Dense>
#include <vector>
#include <iostream>

class NeuralNetwork {
private:
	std::vector<Eigen::MatrixXd> weights;
	Eigen::MatrixXd inputs;
	float sigmoid(float input); // inte klar
public:
	NeuralNetwork(std::vector<int> layerSizes);
	

};
