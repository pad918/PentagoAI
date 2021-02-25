#include "NNTrainer.h"

void NNTrainer::testBackprop()
{
	std::string basePath = "save_";
	int saves = 0;
	//srand(time(NULL));
	std::vector<int> layerSizes;
	layerSizes.push_back(2);
	layerSizes.push_back(10);
	layerSizes.push_back(10);
	layerSizes.push_back(1);
	NeuralNetwork nn(layerSizes);

	//is point inside of circle test
	int hits = 0;
	int total = 0;
	//nn.loadNetwork("save_zero.txt");
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
		if (nn.backpropogation(targets, 0.1)) { hits++; }
		if (++total % 100000 == 0) {
			

			float hitRate = 100.0f * hits / (float)total;
			std::cout << "Hitrate in the past " << total << " tests was: " << hitRate << "%\n";

			/*Draw circle*/
			std::cout << "This is a circle:\n";
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
			}

			//Set vars to zero
			total = 0; 
			hits = 0;
		}
	}
	nn.saveNetwork("save_zero.txt");
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

void NNTrainer::trainAgainstMinmax()
{
	/* Create the neural network 
	
	Inputs: 
		1  - 36 position

	Outputs:
		1  - 36 position
		37 - 44 rotation
	*/
	std::vector<int> layerSizes;
	layerSizes.push_back(108);
	layerSizes.push_back(80);
	layerSizes.push_back(80);
	layerSizes.push_back(80);
	layerSizes.push_back(44);
	NeuralNetwork nn(layerSizes);
	nn.loadNetwork("80by3_minimax_depth3_NEW_MINIMAX1.txt"); //load network
	/* Setup pentago board and minimax ai
		minimax =	player 1
		NN		=	player 2
	*/
	mm::Minimax ai;
	int maxDepth = 3;
	ai.maxDepth = maxDepth; // set minimax deapth
	ptg::PentagoGame pentagoBoard;
	int hits = 0;
	int illegalMoves = 0;
	int trainingNr = 0;
	double costSum=0;
	double bestAvgCost = 1.0f; 
	int testAtBestCost = 0;
	long batchNumber = 17100;// 95000;
	/* Train the neural network */
	for (int i = 0; i < 100000; i++) {

		/* Set training sample (Pantagoboard)*/
		//pentagoBoard = ptg::PentagoGame();
		//int numOfMoves = rand() % 25 + 3;
		//pentagoBoard.setMarble(0, 0, rand() % 2 + 1);
		//for (int i = 0; i < numOfMoves; i++) {
		//	pentagoBoard.setMarble(rand() % 6, rand() % 6, (i % 2 == 0) ? 2 : 1);
		//}

		//New sample method:
		pentagoBoard = ptg::PentagoGame();
		int numOfMoves = rand() % 12 + 2;
		numOfMoves += 1 - numOfMoves % 2; //Only odd numbers  
		for (int k = 0; k < numOfMoves; k++) {
			int type = rand() % 1;
			int len = rand() % 2 + 1;
			int row = rand() % 6;
			int sp = rand() % (6 - len) + rand() % 3;
			switch (type)
			{
			case 0:
				for (int j = 0; j < len; j++) {
					pentagoBoard.setMarble((sp+j)%6, row, (k % 2 == 0) ? 2 : 1);
					//pentagoBoard.printBoard();
				}
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			}
		}
		int numOfRots = rand() % 4;
		for (int k = 0; k < numOfRots; k++) {
			pentagoBoard.rotateSubBoard(rand() % 2, rand() % 2, (rand() % 2 == 0) ? 1 : -1);
		}
		//pentagoBoard.printBoard();

		/* Set inputs from pentago board */
		Eigen::MatrixXd inputs(108, 1);
		inputs.setZero();
		for (int j = 0; j < 36; j++) {
			int player = pentagoBoard.marbleAt(j%6, j/6);
			inputs(j * 3 + player, 0) = 1;
		}
		nn.setInputs(inputs);
		
		/* Use minimax on board and generate target outputs for the network */
		//auto start = std::chrono::high_resolution_clock::now();
		
		ai.clearTables();
		ai.minimax(mth::PentagoMove(), maxDepth, 1, -1000, 1000, pentagoBoard);
		//auto stop = std::chrono::high_resolution_clock::now();
		//auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
		//std::cout << "Time to calc minimax: " << duration.count() << "ms\n";
		int targetMarblePos = ai.bestMove.marblePos.y * 6 + ai.bestMove.marblePos.x;
		int targetRotation = ai.bestMove.rotation.x * 2 + (ai.bestMove.rotation.y + 1) / 2; // OBS! kan vara fel här
		Eigen::MatrixXd targetOutputs(44, 1);
		targetOutputs.setZero();
		
		//Give illegal moves a negative target value
		/*
		for (int x = 0; x < 6; x++) {
			for (int y = 0; y < 6; y++) {
				if (pentagoBoard.marbleAt(x, y) != 0) {
					targetOutputs(y * 6 + x, 0) = -1.00f; //kanske för lågt? kommer den bara försöka undvika allt nu eller?
				}
			}
		}
		*/
		//Set minimax target outputs
		targetOutputs(targetMarblePos, 0) = 1;
		targetOutputs(36 + targetRotation, 0) = 1;
		
		

		/* Backpropagate the network */

		double lr = 1 / (1.0 + 1.0 * std::sqrt(++batchNumber)); // change learning rate here!
		bool wasCorrect = nn.backpropogation(targetOutputs, lr); 
		costSum += nn.calculateCost(targetOutputs);
		hits += wasCorrect ? 1 : 0;

		if (++trainingNr % 100 == 0) {
			std::cout << "-------------------------------------------------------------\n";
			double avgCost = (costSum / 100.0);
			if (avgCost < bestAvgCost) { 
				bestAvgCost = avgCost; 
				testAtBestCost = batchNumber /100;
				nn.saveNetwork("80by3_minimax_depth3_NEW_MINIMAX1.txt");
			}
			
			std::cout << "Hitrate in the last " << trainingNr << " tests = " << (100.0f * hits / (float)trainingNr) << "% | avg cost = " << avgCost << "\n";
			std::cout << "BatchNumber = " << batchNumber << " | LR = " << lr << " | tests since best cost = " << (batchNumber/100-testAtBestCost) << "\n";
			std::cout << "testAtBestCost = " << testAtBestCost << "\n";
			trainingNr = 0;
			costSum = 0.0;
			hits = 0;
			illegalMoves = 0;
		}

		
	}
}
