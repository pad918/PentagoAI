#include "NeuralNetwork.h"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

void NeuralNetwork::sigmoid(Eigen::MatrixXd & input) //Inte testad
{

	//Borde kunna använda vectorization här.
	
	for (int i = 0; i < input.rows()*inputs.cols(); i++) {
		input.array()(i) = 1.0f / (1 + std::pow(2.7182818284f, -input.array()(i)));;
	}
	
}

float NeuralNetwork::calculateError(Eigen::MatrixXd output, Eigen::MatrixXd & target) //KANSKE LÅNGSAM!
{
	output = output - target;
	output = output.array().pow(2);
	output *= 0.5f;
	return output.sum();
}

void NeuralNetwork::backpropogation()
{
	Eigen::MatrixXd targets; //get targets here!!!!
	targets = Eigen::MatrixXd(2, 1);
	targets(0, 0) = 0.0f;
	targets(1, 0) = 1.0f;

	Eigen::MatrixXd outputs = calculateOutputs();
	float error = calculateError(outputs, targets);

	//Output layer is different from other layers.
	//std::cout << "DELATAS\n";
	for (int o = 0; o < networkLayerSizes.back(); o++) { //

		float nodeDelta = outputs(o, 0) * (1.0f - outputs(o, 0)) * (outputs(o, 0) - targets(o, 0));

		for (int h = 0; h < networkLayerSizes[networkLayerSizes.size()-2]; h++) { //Borde vara -2

			float delta = nodeDelta * neurons[neurons.size() - 2](h, 0);
			weights.back()(o, h) -= delta * 0.5f;
			//std::cout << delta << "\n";
		}
	}
	
	//For the rest of the layers
	for (int lay = networkLayerSizes.size() - 2; lay > 0; lay--) {
		
	}
	
}

void NeuralNetwork::printMatrix(Eigen::MatrixXd matrix)
{
	std::cout << matrix << "\n";
}

NeuralNetwork::NeuralNetwork(std::vector<int> layerSizes)
{
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
		//std::cout << "Weight " << i << " = \n" << weights[i] << "\n";

		//Bias values are zero from the start.
		biases[i].setZero();
	}

	//Add neurons
	for (int &size : layerSizes) {
		neurons.push_back(Eigen::MatrixXd(size, 1));
		neurons.back().setZero();
	}
}

double NeuralNetwork::evaluate() 
{
	Eigen::MatrixXd nextLayer;
	nextLayer.noalias() = weights[0] * inputs;
	sigmoid(nextLayer);
	for (int i = 1; i < networkLayerSizes.size() - 1; i++) {
		nextLayer = weights[i] * nextLayer;
		sigmoid(nextLayer);
	}

	//Loss function: TO CHANGE!
	Eigen::MatrixXd preferedValues(nextLayer.rows(), 1); // Ska ändras för att passa AI:n
	if (inputs(0, 0) < inputs(1, 0)) {
		preferedValues(0, 0) = 0;
		preferedValues(1, 0) = 1;
	}
	else {
		preferedValues(0, 0) = 1;
		preferedValues(1, 0) = 0;
	}

	Eigen::MatrixXd dif;
	dif.noalias() = nextLayer - preferedValues;
	dif = dif.array().pow(2);


	return dif.sum();
}

Eigen::MatrixXd NeuralNetwork::calculateOutputs() 
{
	
	for (int i = 1; i < networkLayerSizes.size(); i++) {
		neurons[i] = weights[i-1] * neurons[i-1] + biases[i-1];
		sigmoid(neurons[i]);
	}
	std::cout << "Outputs: \n";
	printMatrix(neurons.back());
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
					saveFile << weights[i](y, x) << "\n";
					//std::cout << i << " " << x  << " " << y << "\n";
				}
			}
		}
		saveFile << "[Biases]\n";
		for (int i = 1; i < networkLayerSizes.size(); i++) {
			for (int j = 0; j < networkLayerSizes[i]; j++) {
				saveFile << biases[i-1](j, 0) << "\n";
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
