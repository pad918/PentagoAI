#include "NNTrainer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <thread>

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


void NNTrainer::createTrainingData(int offset, int stepSize)
{
	//srand(offset*stepSize + time(NULL));
	mth::rand random;
	for (int b = offset; b < 100; b+=stepSize) {
		
		//Create file:
		std::string path = "dataset/" + std::to_string(b * 1000) + ".txt";
		std::ofstream saveFile(path);

		if (saveFile.is_open())
		{
			//create all the samples
			for (int i = 0; i < 100; ++i) {
				auto board = ptg::PentagoGame();

				int numOfMoves = 2 * random.intRand(1, 6);//2 * (rand() % 6 + 1);
				//numOfMoves += 1 - numOfMoves % 2; //Only odd numbers  
				int playerOffset = random.intRand(0, 1);
				for (int k = playerOffset; k < numOfMoves + playerOffset; k++) {
					int type = random.intRand(0, 3);  //rand() % 4;
					int len =  random.intRand(1, 2);  //rand() % 2 + 1;
					int row =  random.intRand(0, 5);// rand() % 6;
					int col =  random.intRand(0, 5);// rand() % 6;
					int sp  =  random.intRand(0, 6-len);// rand() % (6 - len) + rand() % 3;
					switch (type)
					{
					case 0:
						for (int j = 0; j < len; j++) {

							board.setMarble((sp + j) % 6, row, (k % 2 == 0) ? 2 : 1);
							//pentagoBoard.printBoard();
						}
						break;
					case 1:
						for (int j = 0; j < len; j++) {
							board.setMarble(col, (sp + j) % 6, (k % 2 == 0) ? 2 : 1);
							//pentagoBoard.printBoard();
						}
						break;
					case 2:
						for (int j = 0; j < len; j++) {
							board.setMarble((sp + j) % 6, (sp + j) % 6, (k % 2 == 0) ? 2 : 1);
							//pentagoBoard.printBoard();
						}
						break;
					case 3:
						for (int j = 0; j < len; j++) {
							board.setMarble((sp + j) % 6, 5 - ((sp + j) % 6), (k % 2 == 0) ? 2 : 1);
							//pentagoBoard.printBoard();
						}
						break;
					}
				}

				//Create som random noize
				int noiseSamples = random.intRand(6, 14);
				for (int n = 0; n < noiseSamples; ++n) {
					board.setMarble(random.intRand(0, 5), random.intRand(0, 5), 1 + n % 2);
				}

				//Rotate som of the subboards
				int numOfRots = random.intRand(0, 3); // rand() % 4;
				for (int k = 0; k < numOfRots; k++) {
					board.rotateSubBoard(random.intRand(0, 1), random.intRand(0, 1), (random.intRand(0, 1) == 0) ? 1 : -1);
				}

				//Flip subboards randomly
				int flip = random.intRand(0, 2);//rand() & 1;
				if (flip > 0) {
					for (int p = 0; p < 2; ++p) {
						if (flip == 1) {
							auto tmp = board.subBoards[p * 2];
							board.subBoards[p * 2] = board.subBoards[p * 2 + 1];
							board.subBoards[p * 2 + 1] = tmp;
						}
						else {
							auto tmp = board.subBoards[p];
							board.subBoards[p] = board.subBoards[p + 2];
							board.subBoards[p + 2] = tmp;
						}
					}
				}
				//board.printBoard();


				//store board in dataset file
				saveFile << std::to_string(board.getShortHash(0)) << "\n";

				//Find minimax eval of all 288 positions
				mm::Minimax ai;
				ai.maxDepth = 3; // set minimax deapth
				HashTable table;
				for (int y = 0; y < 6; ++y) {
					for (int x = 0; x < 6; ++x) {
						ptg::PentagoGame newBoard = board;
						if (board.marbleAt(x, y) != 0) {
							saveFile << std::to_string(0) << "\n";
							continue;
						}
						newBoard.setMarble(x, y, 1);
						for (int r = 0; r < 8; ++r) {
							int eval = 0;
							newBoard.rotateSubBoard(r / 2, (r % 2 == 0) ? 1 : -1);
							auto key = newBoard.getShortHash(0);
							if (table.highestDepthOfHashNy(key) > 0) {
								eval = table.getValNy(key);
							}
							else {
								eval = ai.minimax2(ai.maxDepth, 1, -10000, 10000, newBoard);
								table.addElementNy(key, eval, 3);
							}
							saveFile << std::to_string(eval) << "\n";
							//std::cout << "ONE MINMAX\n";
							ai.clearTables();
						}
					}
				}
				table.clear();
				std::cout << "created one sample in thread: " << offset << " | progress = " << i << "/100 " << " | board key = " << board.getShortHash(0) << "\n";
			}
			std::cout << "saved dataset file with batchnumber: " << b << " | path = " << path << "\n";
			saveFile.close();
		}
	}
}

void NNTrainer::trainAgainstDataset()
{
	std::vector<int> layerSizes;
	layerSizes.push_back(108);
	layerSizes.push_back(80);
	layerSizes.push_back(80);
	layerSizes.push_back(80);
	layerSizes.push_back(288); //8 rotations times 36 positions
	NeuralNetwork nn(layerSizes);
	nn.loadNetwork("80by3_dataset2_depth3_overtrained.txt"); //load network

	//Load training data:
	Dataset dataset("dataset/data2/", 65);


	//train the network:
	ptg::PentagoGame pentagoBoard;
	int hits = 0;
	int illegalMoves = 0;
	int trainingNr = 0;
	double costSum = 0;
	double bestAvgCost = 8.85; // INFINITY;
	int testAtBestCost = 0;
	long batchNumber = 1100000;// 95000;
	int sizeOfBatch = 10000;

	for (int i = 0; i < 3000000; i++) {
		//Load pentago board from training data
		auto key = dataset.getBoard(i);
		pentagoBoard.loadBoardFromHash(key);

		/* Set inputs from pentago board */
		Eigen::MatrixXd inputs(108, 1);
		inputs.setZero();
		for (int j = 0; j < 36; j++) {
			int player = pentagoBoard.marbleAt(j % 6, j / 6);
			inputs(j * 3 + player, 0) = 1;
		}
		nn.setInputs(inputs);
		
		//Set targets for backpropgataion
		Eigen::MatrixXd targetOutputs(288, 1);
		targetOutputs.setZero();
		auto dataTargets = dataset.getTargets(i);
		for (int t = 0; t < 288; ++t) {
			targetOutputs(t, 0) = dataTargets[t];
		}


		/* Backpropagate the network */

		double lr = 1 / (1.0 + 1.0 * std::sqrt(++batchNumber)); // change learning rate here!
		bool wasCorrect = nn.backpropogation(targetOutputs, lr);
		costSum += nn.calculateCost(targetOutputs);

		hits += wasCorrect ? 1 : 0;

		if (++trainingNr % sizeOfBatch == 0) {
			std::cout << "-------------------------------------------------------------\n";
			double avgCost = (costSum / (double)sizeOfBatch);
			if (avgCost < bestAvgCost) {
				bestAvgCost = avgCost;
				testAtBestCost = batchNumber / sizeOfBatch;
				nn.saveNetwork("80by3_dataset2_depth3_overtrained_delux.txt");
			}

			std::cout << "Hitrate in the last " << trainingNr << " tests = " << ((float)sizeOfBatch * hits / (float)trainingNr) << "% | avg cost = " << avgCost << "\n";
			std::cout << "BatchNumber = " << batchNumber << " | LR = " << lr << " | tests since best cost = " << (batchNumber / sizeOfBatch - testAtBestCost) << "\n";
			std::cout << "testAtBestCost = " << testAtBestCost << "\n";
			trainingNr = 0;
			costSum = 0.0;
			hits = 0;
			illegalMoves = 0;
		}

	}

}

void NNTrainer::generateData(int numThreads)
{
	//Start threads
	std::thread threads[4];
	std::cout << "Creating " << numThreads << " treads...\n";

	for (int i = 0; i < 4; i++) {
		threads[i] = std::thread(&NNTrainer::createTrainingData, this, i, 4);
	}
	for (int i = 0; i < 4; ++i) {
		threads[i].join();
		std::cout << "Thread " << i << " finnished\n";
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
		ai.minimax2(maxDepth, 1, -10000, 10000, pentagoBoard);
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
