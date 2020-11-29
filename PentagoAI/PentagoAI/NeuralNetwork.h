#pragma once
#include <Eigen/Dense>
#include <vector>
#include <iostream>
#include <string>

class NeuralNetwork {
private:
	//Object and vars
	std::vector<Eigen::MatrixXd> weights;
	std::vector<Eigen::MatrixXd> biases; //Inte implmeenteread än
	Eigen::MatrixXd inputs;
	std::vector<int> networkLayerSizes;
	//Functions
	void sigmoid(Eigen::MatrixXd & input); 


public:
	NeuralNetwork(std::vector<int> layerSizes);
	double evaluate();
	void saveNetwork(std::string name); // INTE TESTAD
	void loadNetwork(std::string name);
};
