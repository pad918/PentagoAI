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
	Eigen::MatrixXd inputs; // To be removed, replaced by neurons[0]
	std::vector<int> networkLayerSizes;

	//Functions
	void sigmoid(Eigen::MatrixXd & input);
	void sigmoidDerivative(Eigen::MatrixXd & input);
	void rectifier(Eigen::MatrixXd & input);
	void rectifierDerivative(Eigen::MatrixXd & input);

	float calculateCost(Eigen::MatrixXd output, Eigen::MatrixXd & target); //Inte testad
	

	//Debug functions:
	void printMatrix(Eigen::MatrixXd matrix);

public:
	NeuralNetwork(std::vector<int> layerSizes);
	double evaluate(); // Old function 
	Eigen::MatrixXd calculateOutputs();
	void saveNetwork(std::string name); // Kan förekomma buggar
	void loadNetwork(std::string name);	// Kan förekomma buggar
	bool setInputs(Eigen::MatrixXd inputsValues);
	bool backpropogation(Eigen::MatrixXd targets, double learningRate);

	//Debug functions:
	void benchmarkSigmoid(int times);
};
