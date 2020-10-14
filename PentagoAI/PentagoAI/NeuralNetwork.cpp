#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork(std::vector<int> layerSizes)
{
	std::cout << "Creating neural network...\n\n";
	if (layerSizes.size() < 3) { std::cout << "Incrorrect input size\n"; return; }
	inputs = Eigen::MatrixXd(layerSizes[0], 1);
	inputs.setOnes();

	//Set all weights to a random number in the range 0.0 to 1.0
	for (int i = 0; i < layerSizes.size() - 1; i++) {
		weights.push_back(Eigen::MatrixXd(layerSizes[i + 1], layerSizes[i]));
		weights[i].setRandom();
		Eigen::MatrixXd a(layerSizes[i + 1], layerSizes[i]);
		a.setOnes();
		weights[i] += a;
		weights[i] *= 0.5f;
		std::cout << "Weight " << i << " = \n" << weights[i] << "\n";
	}
}
