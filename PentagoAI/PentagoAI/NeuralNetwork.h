#pragma once
#include <Eigen/Dense>
#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <random>
#include <iterator>
#include <iomanip>

class NeuralNetwork {
private:
	//Objects and vars
	std::vector<Eigen::MatrixXd> weights;
	std::vector<Eigen::MatrixXd> biases;
	std::vector<Eigen::MatrixXd> neurons;
	Eigen::MatrixXd lastOutputs;
	Eigen::MatrixXd inputs; // To be removed, replaced by neurons[0]
	std::vector<int> networkLayerSizes;

	//Functions
	void sigmoid(Eigen::MatrixXd & input);
	void sigmoidDerivative(Eigen::MatrixXd & input);
	void rectifier(Eigen::MatrixXd & input);
	void rectifierDerivative(Eigen::MatrixXd & input);
	

	//Debug functions:
	void printMatrix(Eigen::MatrixXd matrix);

public:
	NeuralNetwork(std::vector<int> layerSizes);
	Eigen::MatrixXd calculateOutputs();
	void saveNetwork(std::string name); 
	void loadNetwork(std::string name);	
	bool setInputs(Eigen::MatrixXd inputsValues);
	bool backpropogation(Eigen::MatrixXd targets, double learningRate);
	double calculateCost(Eigen::MatrixXd & target); 

	//Debug functions:
	void benchmarkSigmoid(int times);
};
