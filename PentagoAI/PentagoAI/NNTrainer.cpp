#include "NNTrainer.h"

void NNTrainer::testBackprop()
{
	std::string basePath = "save_";
	int saves = 0;
	srand(time(NULL));
	std::vector<int> layerSizes;
	layerSizes.push_back(2);
	layerSizes.push_back(10);
	layerSizes.push_back(10); 
	layerSizes.push_back(10); 
	layerSizes.push_back(1);
	NeuralNetwork nn(layerSizes);
	//nn.loadNetwork("smallTest.txt");
	//nn.saveNetwork("nameless.txt");

	/*
	ideas:

		--> is point inside of circle
		is point under function f(x)
		if x^2 + y^2 < 1 
	
	*/


	//is point inside of circle test
	int hits = 0;
	int total = 0;
	nn.loadNetwork("save_zero.txt");
	for (int i = 0; i < 1000000; i++) {
		Eigen::MatrixXd inputs(2, 1);
		inputs(0, 0) = (rand() % 10000) / 10000.0f;
		inputs(1, 0) = (rand() % 10000) / 10000.0f;
		Eigen::MatrixXd targets(1, 1);


		/*Calculate correct ans and set target*/
		float dist = std::sqrt(std::pow((inputs(0, 0) - 0.5f), 2) + std::pow((inputs(1, 0) - 0.5f), 2));
		bool correctAns = dist < 0.5f;
		targets(0, 0) = correctAns ? 1.0f : 0.0f;

		/*Do the backprop*/
		nn.setInputs(inputs);
		nn.calculateOutputs();
		if (nn.backpropogation(targets)) { hits++; }
		if (++total % 100000 == 0) {
			nn.saveNetwork("save_zero.txt");

			float hitRate = 100.0f * hits / (float)total;
			std::cout << "Hitrate in the past " << total << " tests was: " << hitRate << "%\n";

			/*Draw circle*/
			/*std::cout << "This is a circle:\n";
			for (float x = 0; x <= 1.01f; x += 0.025f) {
				for (float y = 0; y <= 1.01f; y += 0.025f) {
					inputs(0, 0) = x;
					inputs(1, 0) = y;
					nn.setInputs(inputs);
					auto outputs = nn.calculateOutputs();
					bool isInCircle = outputs(0, 0) > 0.5f;
					std::cout << isInCircle << " ";
				}
				std::cout << "\n";
			}*/

			//Set vars to zero
			total = 0; 
			hits = 0;
		}
	}

	/* Make it draw what is thinks a circle looks like */
	Eigen::MatrixXd inputs(2, 1);
	
	std::cout << "This is a circle:\n";
	for (float x = 0; x <= 1.01f; x += 0.05f) {
		for (float y = 0; y <= 1.01f; y += 0.05f) {
			inputs(0, 0) = x;
			inputs(1, 0) = y;
			nn.setInputs(inputs);
			auto outputs = nn.calculateOutputs();
			bool isInCircle = outputs(0, 0) > 0.5f;
			std::cout << isInCircle << " ";
		}
		std::cout << "\n";
	}

}
