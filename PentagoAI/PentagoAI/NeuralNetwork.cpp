﻿#include "NeuralNetwork.h"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

void NeuralNetwork::activationFunction(Eigen::MatrixXd & input)
{
	tanh(input);
}

void NeuralNetwork::activationFunctionDerivative(Eigen::MatrixXd & input)
{
	tanhDerivative(input);
}

void NeuralNetwork::sigmoid(Eigen::MatrixXd & input) //Testad
{

	//Borde kunna använda vectorization här.
	
	for (int i = 0; i < input.rows()*input.cols(); i++) {
		input.array()(i) = 1.0f / (1 + std::pow(2.7182818284f, -input.array()(i)));;
	}
	
}

void NeuralNetwork::sigmoidDerivative(Eigen::MatrixXd & input) //Testad
{
	sigmoid(input);
	Eigen::MatrixXd allOnes = input; // För att få rätt storlek
	allOnes.setOnes();
	Eigen::MatrixXd output = allOnes - input;
	output.array() = output.array() * input.array();
	input = output;
}

void NeuralNetwork::rectifier(Eigen::MatrixXd & input)
{
	//for (int i = 0; i < input.rows()*input.cols(); i++) {
	//	if (!isfinite(input.array()(i))) {
	//		std::cout << "skedde inann i = " << i << "\n";
	//		break;
	//	}
	//}
	input.array() = input.array().max(0.0);
	//for (int i = 0; i < input.rows()*input.cols(); i++) {
	//	if (!isfinite(input.array()(i))) {
	//		std::cout << "skedde efter i = " << i << "\n";
	//		break;
	//	}
	//}
}

void NeuralNetwork::rectifierDerivative(Eigen::MatrixXd & input)
{
	rectifier(input);
	for (int i = 0; i < input.rows()*input.cols(); i++) {

		input.array()(i) = (input.array()(i) >= 0) ? 1.0 : 0.0;

	}
}

void NeuralNetwork::swich(Eigen::MatrixXd & input)
{
	/* https://www.geeksforgeeks.org/ml-swish-function-by-google-in-keras */
	auto tmp = input;
	sigmoid(tmp);
	input.array() = input.array() * tmp.array();
}

void NeuralNetwork::swichDerivative(Eigen::MatrixXd & input)
{
	auto swiched = input; /*y*/
	auto sigmoided = input; /*sig(x)*/
	sigmoid(sigmoided);
	swich(swiched);
	//inputs.array() = swiched.array() + sigmoided.array() * (1.0 - swiched.array()); //?
	for (int i = 0; i < input.rows()*input.cols(); i++) {
		double f1 = (1.0 - swiched.array()(i));
		double f2 = sigmoided.array()(i) * f1;
		double f3 = swiched.array()(i) * f2;
		input.array()(i) = f3;
		//input.array()(i) = swiched.array()(i) + (sigmoided.array()(i) * (1.0 - swiched.array()(i)));
	}
}

void NeuralNetwork::reluLoss(Eigen::MatrixXd & input)
{
	for (int i = 0; i < input.rows()*input.cols(); i++) {
		if (input.array()(i) < 0) {
			double tmp = input.array()(i);
			tmp *= 0.01;
			input.array()(i) = tmp;
		}
	}
}

void NeuralNetwork::reluLossDerivative(Eigen::MatrixXd & input)
{
	for (int i = 0; i < input.rows()*input.cols(); i++) {
		double tmp = input.array()(i);
		tmp = tmp > 0.0 ? 1.0 : 0.0;
		input.array()(i) = tmp;

	}
}

void NeuralNetwork::tanh(Eigen::MatrixXd & input)
{
	for (int i = 0; i < input.rows()*input.cols(); i++) {
			double tmp = input.array()(i);
			tmp = (std::pow(2.7182818284f, tmp) - std::pow(2.7182818284f, -tmp)) / (std::pow(2.7182818284f, tmp) + std::pow(2.7182818284f, -tmp));
			input.array()(i) = tmp;
	}
}

void NeuralNetwork::tanhDerivative(Eigen::MatrixXd & input)
{
	for (int i = 0; i < input.rows()*input.cols(); i++) {
		double tanhTmp = input.array()(i);
		tanhTmp = (std::pow(2.7182818284f, tanhTmp) - std::pow(2.7182818284f, -tanhTmp)) / (std::pow(2.7182818284f, tanhTmp) + std::pow(2.7182818284f, -tanhTmp));
		tanhTmp = 1.0 - (tanhTmp * tanhTmp);
		input.array()(i) = tanhTmp;
	}
}

double NeuralNetwork::calculateCost(Eigen::MatrixXd & target) //KANSKE LÅNGSAM!
{
	auto output = lastOutputs;
	output = output - target;
	output = output.array().pow(2);
	output *= 0.5;
	return output.sum();
}

bool NeuralNetwork::backpropogation(Eigen::MatrixXd targets, double learningRate)
{
	bool wasCorrect = false;
	Eigen::MatrixXd outputs = calculateOutputs();
	lastOutputs = outputs;
	if (targets.cols() != outputs.cols() || targets.rows() != outputs.rows()) {
		std::cout << "ERROR: targets and outputs matrix are not of the same size\n";
	}
	/* Setting outputs to one or zero depending on chance [NAME OF THIS?] */
	//double weightsChanses[44]; 
	//for (int k=0; k<44; k++)
	//	weightsChanses[k] = outputs(k, 0);
	//std::random_device rd;
	//std::mt19937 gen(rd()); // En långsam men nogrann slumpmetod
	//std::discrete_distribution<> d(std::begin(weightsChanses), std::end(weightsChanses) - 8);
	//std::discrete_distribution<> f(std::begin(weightsChanses) + 36, std::end(weightsChanses));
	//
	//std::map<int, int> position, rotation;
	//++position[d(gen)];
	//++rotation[f(gen)];
	//outputs.setZero();
	//outputs(position.begin()->first, 0) = 1;
	//outputs(rotation.begin()->first + 36, 0) = 1;

	/* Calculate ouput error and outputinput*/
	Eigen::MatrixXd outputError = targets - outputs; // Eo
	Eigen::MatrixXd outputInput = weights.back() * neurons[neurons.size()-2] + biases.back();
	activationFunctionDerivative(outputInput); 
	outputError.array() = outputError.array() * outputInput.array();

	//update weights
	Eigen::MatrixXd weightDerivate = outputError * neurons[neurons.size() - 2].transpose();
	//weightDerivate.array() = weightDerivate.array().min(0.1);
	weights.back() += weightDerivate * learningRate;

	//Update biases
	biases.back() += outputError * learningRate;

	//Hidden layers 
	Eigen::MatrixXd prevLayerError = outputError;

	//Print avrage error for differant layers...

	for (int i = neurons.size() - 2; i >= 1; i--) {

		//Calculate layer error
		Eigen::MatrixXd transposedWeight = weights[i].transpose(); // weights.back().transpose() | next layer weights
		Eigen::MatrixXd hiddenError = transposedWeight * prevLayerError; // output error

		//Calculate layer inputs
		Eigen::MatrixXd hiddenInput = weights[i-1] * neurons[i-1] + biases[i-1];  
		activationFunctionDerivative(hiddenInput); 
		hiddenError.array() *= hiddenInput.array(); 

		Eigen::MatrixXd wd = hiddenError * neurons[i-1].transpose(); //HIDDDEN ERROR INFINITY!
		//wd.array() = wd.array().min(0.1);
		weights[i - 1] += wd * learningRate;
		biases[i - 1] += hiddenError * learningRate;

		prevLayerError = hiddenError;
	}
	//DEBUG:
	//std::cout << "lay1 = " << sums[0] << " Lay2 = " << sums[1] << " Lay3 = " << sums[2] << "\n";

	int maxIndex = 0;
	double maxVal = 0;
	for (int i = 0; i < outputs.rows(); i++) {
		maxIndex = maxVal < outputs(i, 0) ? i : maxIndex;
		maxVal = maxVal < outputs(i, 0) ? outputs(i, 0) : maxVal;
	}
	wasCorrect = targets(maxIndex, 0) == 1;
	return (wasCorrect);

}

void NeuralNetwork::printMatrix(Eigen::MatrixXd matrix)
{
	std::cout << matrix << "\n";
}

NeuralNetwork::NeuralNetwork(std::vector<int> layerSizes)
{
	for(int i = 0; i<3; i++)
		sums[i] = 0;

	networkLayerSizes = layerSizes;
	std::cout << "Creating neural network...\n\n";
	if (layerSizes.size() < 3) { std::cout << "Incrorrect input size\n"; return; }
	inputs = Eigen::MatrixXd(layerSizes[0], 1);
	inputs.setOnes();

	for (int i = 0; i < layerSizes.size() - 1; i++) {
		//Add weight matrixes
		weights.push_back(Eigen::MatrixXd(layerSizes[i + 1], layerSizes[i]));

		//add rows of biases.
		biases.push_back(Eigen::MatrixXd(layerSizes[i + 1], 1));

		//Randomize default weight values. 
		weights[i].setRandom();
		
		
		weights[i] *= 0.075; //0.05 | 0.82
		//weights[i] *= std::sqrt(2.0 / networkLayerSizes[i]);
		//std::cout << "Weight " << i << " = \n" << weights[i] << "\n";

		//XAVIER RANDOM
		//weights[i] *= std::sqrt(6.0 / (double)(layerSizes[i] + layerSizes[i+1])); //0.05

		//XAVIER NORMAL
		//std::default_random_engine generator;
		//std::normal_distribution<double> distribution(0.0, std::sqrt(2.0 / networkLayerSizes[i]));
		//for (int k = 0; k < weights[i].size(); k++) {
		//	weights[i].array()(k) = distribution(generator);
		//}

		//Bias values are zero from the start.
		biases[i].setZero();
	}

	//Add neurons
	for (int &size : layerSizes) {
		neurons.push_back(Eigen::MatrixXd(size, 1));
		neurons.back().setZero();
	}
}

Eigen::MatrixXd NeuralNetwork::calculateOutputs() 
{
	
	for (int i = 1; i < networkLayerSizes.size(); i++) {

		neurons[i] = weights[i-1] * neurons[i-1] + biases[i-1];
		
		activationFunction(neurons[i]); 
	}
	return neurons.back();
	
}

void NeuralNetwork::saveNetwork(std::string name)
{
	std::ofstream saveFile("networks/" + name); 
	if (saveFile.is_open())
	{
		saveFile << "[NN saver V0.001]\n";

		//Write file properties

		//write neuralnetwork properties
		saveFile << "[LayerSizes]\n";
		for (int i = 0; i < networkLayerSizes.size(); i++) {
			saveFile << networkLayerSizes[i] << "\n";
		}
		saveFile << "[Weights]\n";
		for (int i = 0; i < weights.size(); i++) {
			for (int x = 0; x < networkLayerSizes[i]; x++) {
				for (int y = 0; y < networkLayerSizes[i + 1]; y++) {
					saveFile << std::setprecision(16) << weights[i](y, x) << "\n";
					//std::cout << i << " " << x  << " " << y << "\n";
				}
			}
		}
		saveFile << "[Biases]\n";
		for (int i = 1; i < networkLayerSizes.size(); i++) {
			for (int j = 0; j < networkLayerSizes[i]; j++) {
				saveFile << std::setprecision(16)  << biases[i-1](j, 0) << "\n";
			}
		}
		saveFile.close();
		std::cout << "Saved network to file: " << ("networks/" + name) << "\n";
	}
	else { std::cout << "Unable to open file\n"; }

}

void NeuralNetwork::loadNetwork(std::string name)
{
	std::cout << "\n-------\nLoading network form file: " + ("networks/" + name + "\n");
	
	networkLayerSizes.clear();
	biases.clear();
	weights.clear();

	std::vector<float> biasList, weightList;
	int readingMode = 0;
	bool skipValueChecking = true;
	std::string line;
	std::ifstream loadFile("networks/" + name);
	
	//Read all data from the file 
	if (loadFile.is_open())
	{
		while (getline(loadFile, line))
		{
			if (line.size() > 0) {
				skipValueChecking = false;
				if (line.at(0) == '[') {
					if (line.compare("[LayerSizes]") == 0) {
						readingMode = 1;
						skipValueChecking = true;
					}
					else if (line.compare("[Weights]") == 0) {
						readingMode = 2;
						skipValueChecking = true;
					}
					else if (line.compare("[Biases]") == 0) {
						readingMode = 3;
						skipValueChecking = true;
					}
					else {
						std::cout << "Using NN version: " << line << "\n";
						readingMode = 0;
						skipValueChecking = true;
					}
				}
				if (!skipValueChecking) {
					switch (readingMode) {
					case 0:
						break;
					case 1:
						networkLayerSizes.push_back(std::stoi(line));
						break;
					case 2:
						weightList.push_back(std::stof(line));
						break;
					case 3:
						biasList.push_back(std::stof(line));
						break;
					}

				}
			}
		}
		loadFile.close();
	}
	else {
		std::cout << "Error opeening datafile: " << ("networks/" + name + "\n");
		return;
	}

	//Create weight matrix och bias matrix and fill the matrices with the data from the file.
	int listPosWights = 0;
	int listPosBiases = 0;
	for (int i = 0; i < networkLayerSizes.size() - 1; i++) {
		//Add weight matrixes
		weights.push_back(Eigen::MatrixXd(networkLayerSizes[i + 1], networkLayerSizes[i]));
		
		//Set the values of the weights
		for (int x = 0; x < networkLayerSizes[i]; x++) {
			for (int y = 0; y < networkLayerSizes[i + 1]; y++) {
				weights[i](y, x) = weightList[listPosWights];
				listPosWights++;
			}
		}

		//add rows of biases.
		biases.push_back(Eigen::MatrixXd(networkLayerSizes[i + 1], 1));

		//Set the values of the bisases
		for (int j = 0; j < networkLayerSizes[i+1]; j++) {
			biases[i](j, 0) = biasList[listPosBiases];
			listPosBiases++;
		}
	}

}

bool NeuralNetwork::setInputs(Eigen::MatrixXd inputsValues)
{
	if (inputsValues.rows() != networkLayerSizes[0] || inputsValues.cols() != 1) {
		std::cout << "Error: input values are formated incorrecly. (cols!=1 or rows != networks first layer size)\n";
		return false;
	}
	neurons[0] = inputsValues;
	return true;
}

void NeuralNetwork::benchmarkSigmoid(int times)
{
	Eigen::MatrixXd nextLayer;
	nextLayer.noalias() = weights[0] * inputs + biases[0];
	sigmoid(nextLayer);
	for (int i = 1; i < networkLayerSizes.size() - 2; i++) {
		nextLayer = weights[i] * nextLayer + biases[i];
		sigmoid(nextLayer);
	}
	auto t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < times; i++) {
		sigmoid(nextLayer);
	}
	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	std::cout << "Benchmark complete: " << times << " sigmoids in " << duration << "ms\n";
}
