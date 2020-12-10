#pragma once
#include <Eigen/Dense>
#include <vector>
#include <iostream>
#include <string>

class NeuralNetwork {
private:
	//Object and vars
	std::vector<Eigen::MatrixXd> weights;
	std::vector<Eigen::MatrixXd> biases; //Inte implmenteread än
	Eigen::MatrixXd inputs;
	std::vector<int> networkLayerSizes;
	//Functions
	void sigmoid(Eigen::MatrixXd & input); 

	//Debug functions:
	void printMatrix(Eigen::MatrixXd matrix);

public:
	NeuralNetwork(std::vector<int> layerSizes);
	double evaluate(); // Old function 
	Eigen::MatrixXd calculateOutputs();
	void saveNetwork(std::string name); // Kan förekomma buggar
	void loadNetwork(std::string name);	// Kan förekomma buggar
	bool setInputs(Eigen::MatrixXd inputsValues);

	//Debug functions:
	void benchmarkSigmoid(int times);
};
