#include "PlayGame.h"
#include <chrono>

GameHandler::GameHandler()
{

}

void GameHandler::playGame(int playerOneType, int playerTwoType)
{
	/*
		player         = 0
		minimax        = 1
		neural network = 2
	*/
	//initialize players and variables
	playerOne = new HumanPlayer;
	//playerTwo = new MinimaxPlayer;
	playerTwo = new NNPlayer;

	int hasWon = 0;
	ptg::PentagoGame board;

	for (int i = 0; i < 18; i++) {
		//Player one's move
		std::cout << "\n";
		board.printBoard();
		playerOne->doMove(board);
		hasWon = board.hasWonFast();
		if (hasWon != 0) {
			break;
		}
		
		//Player two's turn
		std::cout << "\n";
		board.printBoard();
		playerTwo->doMove(board);
		hasWon = board.hasWonFast();
		if (hasWon != 0) {
			break;
		}
	}

	std::cout << "\n\n";
	//system("cls");
	board.printBoard();
	std::cout << "  Player: " << hasWon << " WON!\n";
}

void GameHandler::playAgainstMinimax(ptg::PentagoGame &board, int depth)
{
	mm::Minimax ai;
	ai.maxDepth = depth;
	int hasWon = 0;
	int aiPoints = 0;
	int plTurn = 0;
	for (int i = 0; i < 18; i++) {
		//Player move
		std::cout << "\n";
		//system("cls"); //REMOVE SHITTY WINDOWS ONLY SHIT!
		board.printBoard();
		//std::cout << "\n \n AI BUFFER: " << (ai.hashTableMax.hashList.size() + ai.hashTableMin.hashList.size()) << "\n\n"; SKA INTE VARA KOMMENTERAD
		int x, y;
		std::cout << "Player's turn. " << "| Set marble: x:y : ";
		std::cin >> x >> y;
		board.setMarble(x, y, 2);
		hasWon = board.hasWonFast();
		if (hasWon != 0) {
			break;
		}
		std::cout << "\n";
		//system("cls"); //REMOVE SHITTY WINDOWS ONLY SHIT!
		board.printBoard();
		std::cout << "\n";
		int dir;
		std::cout << "Player's turn. " << "| Rotate sub board: x:y:dir : ";
		std::cin >> x >> y >> dir;
		board.rotateSubBoard(x, y, dir);
		hasWon = board.hasWonFast();
		if (hasWon != 0) {
			break;
		}
		//Ai move
		std::cout << "\n";
		//system("cls"); //REMOVE SHITTY WINDOWS ONLY SHIT!
		board.printBoard();
		std::cout << "\n";
		std::cout << "minimax AI's turn...";
		auto start = std::chrono::high_resolution_clock::now();
		aiPoints = ai.minimax(mth::PentagoMove(), depth, 1, -1000, 1000, board); // player = 1
		auto stop = std::chrono::high_resolution_clock::now();
		aiPoints = ai.testVal;
		if (board.marbleAt(ai.bestMove.marblePos.x, ai.bestMove.marblePos.y) != 0) {
			std::cout << "PLACING OVER EXISTING MARBLE...\n";
			std::cout << "Placing pos = " << ai.bestMove.marblePos.x << " " << ai.bestMove.marblePos.y << " | rot: " << ai.bestMove.rotation.x << " " << ai.bestMove.rotation.y << "\n";
			break;
		}
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
		std::cout << "Time to calc: " << duration.count() << "ms\n";
		board.setMarble(ai.bestMove.marblePos.x, ai.bestMove.marblePos.y, 1);
		board.rotateSubBoard(ai.bestMove.rotation.x, ai.bestMove.rotation.y);
		hasWon = board.hasWonFast();
		if (hasWon != 0) {
			break;
		}
	}
	
	std::cout << "\n\n";
	//system("cls");
	board.printBoard();
	std::cout << "  Player: " << hasWon << " WON!\n";
}

void HumanPlayer::doMove(ptg::PentagoGame & board)
{
	int x, y, rX, rY, dir;
	std::cout << "Player's turn. " << "| Set marble: x y \n";
	std::cin >> x >> y;
	std::cout << "\nRotation: x y dir\n";
	std::cin >> rX >> rY >> dir;
	board.setMarble(x, y, 2);
	board.rotateSubBoard(rX, rY, dir);
}

void MinimaxPlayer::doMove(ptg::PentagoGame & board)
{
	maxDepth = 4;
	ai.maxDepth = maxDepth; // borde flytta till där den inizieras;
	std::cout << "\n";
	std::cout << "minimax AI's turn...";
	auto start = std::chrono::high_resolution_clock::now();
	ai.minimax(mth::PentagoMove(), maxDepth, 1, -1000, 1000, board); // player = 1
	auto stop = std::chrono::high_resolution_clock::now();
	if (board.marbleAt(ai.bestMove.marblePos.x, ai.bestMove.marblePos.y) != 0) {
		std::cout << "PLACING OVER EXISTING MARBLE...\n";
		std::cout << "Placing pos = " << ai.bestMove.marblePos.x << " " << ai.bestMove.marblePos.y << " | rot: " << ai.bestMove.rotation.x << " " << ai.bestMove.rotation.y << "\n";
		return;
	}
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << "Time to calc: " << duration.count() << "ms\n";
	board.setMarble(ai.bestMove.marblePos.x, ai.bestMove.marblePos.y, 1);
	board.rotateSubBoard(ai.bestMove.rotation.x, ai.bestMove.rotation.y);
}

NNPlayer::NNPlayer()
{
	std::vector<int> layerSizes; 
	layerSizes.push_back(10);
	layerSizes.push_back(10);
	layerSizes.push_back(10);
	layerSizes.push_back(10);
	layerSizes.push_back(10);
	nn = new NeuralNetwork(layerSizes);
	nn->loadNetwork("80by3_minimax_depth3_network.txt");
}

void NNPlayer::doMove(ptg::PentagoGame & board)
{
	std::cout << "\n";
	std::cout << "Neural network is choosing its move...\n";
	//Set inputs
	Eigen::MatrixXd inputs(108, 1);
	inputs.setZero();
	for (int j = 0; j < 36; j++) {
		int player = board.marbleAt(j % 6, j / 6);
		inputs(j * 3 + player, 0) = 1;
	}
	nn->setInputs(inputs);

	//Generate outputs
	auto outputs = nn->calculateOutputs();
	double marbleVal=0;
	int maxMarbleValId=0;
	for (int i = 0; i < 36; i++) {
		if (outputs(i, 0) > marbleVal) {
			marbleVal = outputs(i, 0);
			maxMarbleValId = i;
		}
	}
	double rotationVal=0;
	int maxrotationValId=0;
	for (int i = 0; i < 36; i++) {
		if (outputs(i, 0) > rotationVal) {
			rotationVal = outputs(i, 0);
			maxrotationValId = i;
		}
	}

	//Make move
	if (board.marbleAt(maxMarbleValId % 6, maxMarbleValId / 6) != 0) {
		std::cout << "Det neurala närverket är förvirrat och försökte fuska... (lägga på annan pjäs)\n";
	}
	board.setMarble(maxMarbleValId%6, maxMarbleValId/6, 1);
	board.rotateSubBoard(maxrotationValId/2, maxrotationValId%2==0 ? 1 : -1); //OKLART / Kanske fel här!
}
