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
	//playerOne = new NNPlayer;
	playerTwo = new MinimaxPlayer;
	//playerTwo = new NNPlayer;

	int hasWon = 0;
	ptg::PentagoGame board;

	window = new sf::RenderWindow(sf::VideoMode(500, 500), "PENTAGO GAME");;
	GUI gameGUI;

	//Polling vars
	int frameCounter=0;
	bool mouseReleasedThisFrame = false;

	//Create frame
	while (window->isOpen()) {
		mouseReleasedThisFrame = false;
		sf::Event event;
		while (window->pollEvent(event))
		{
			switch (event.type) {
			case sf::Event::Closed:
				window->close(); 
			case sf::Event::MouseButtonReleased:
				mouseReleasedThisFrame = true;
				break;
			default:
				break;
			}

		}

		window->clear();
		gameGUI.update(*window, board);
		window->display();

		int pollingPlayer = 2 * playerTwo->isPollingInput + playerOne->isPollingInput;
		if (pollingPlayer>0) {
			//Do the polling
			if (mouseReleasedThisFrame) {
				if (gameGUI.poll(*window, board)) {
					if (gameGUI.wasMoveLegal) {
						board.rotateSubBoard(gameGUI.rotateBoard.x, gameGUI.rotateBoard.y, gameGUI.rotateBoard.z); //ändra
						playerOne->isPollingInput = false;
						playerTwo->isPollingInput = false;
					}
				}
				else {
					if (!gameGUI.wasMoveLegal) {
						std::cout << "Move is illegal\n";
					}
					else {
						if (pollingPlayer == 1)
							board.setMarble(gameGUI.setMarblePos.x, gameGUI.setMarblePos.y, 2);
						else
							board.setMarble(gameGUI.setMarblePos.x, gameGUI.setMarblePos.y, 1);
					}
				}
				hasWon = board.hasWonFast();
				if (hasWon != 0) {
					std::cout << "player " << hasWon << "Won!" << std::endl;
				}
			}
		}
		else {
			switch (frameCounter++ % 2)
			{
			case 0:
				//Player one's move
				std::cout << "\n";
				//board.printBoard();
				playerOne->doMove(board);
				hasWon = board.hasWonFast();
				if (hasWon != 0) {
					std::cout << "player " << hasWon << "Won!" << std::endl;
				}
				break;
			case 1:
				//Player two's turn
				std::cout << "\n";
				//board.printBoard();
				playerTwo->doMove(board);
				hasWon = board.hasWonFast();
				if (hasWon != 0) {
					std::cout << "player " << hasWon << "Won!" << std::endl;
				}
				break;
			}
		}

		window->clear();
		gameGUI.update(*window, board);
		window->display();
	
	}
	std::cout << "Spelet avslutades av okännd anledning\n";
	/*
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
	*/
}

void HumanPlayer::doMove(ptg::PentagoGame & board)
{
	isPollingInput = true;
	/*int x, y, rX, rY, dir;
	// GAMMAL KOD
	std::cout << "Player's turn. " << "| Set marble: x y \n";
	std::cin >> x >> y;
	std::cout << "\nRotation: x y dir\n";
	std::cin >> rX >> rY >> dir;
	
	board.setMarble(x, y, 2);
	board.rotateSubBoard(rX, rY, dir);*/
}

void MinimaxPlayer::doMove(ptg::PentagoGame & board)
{
	ai.maxDepth = maxDepth; // borde flytta till där den inizieras;
	std::cout << "\n";
	std::cout << "minimax AI's turn...\n";

	
	//ai.ISUSINGHASHTABLE = false;
	//int scoreNotUsing = ai.minimax(mth::PentagoMove(), maxDepth, 1, -1000, 1000, board); // player = 1
	//auto pos1 = ai.bestMove.marblePos;
	//long a = ai.DEBUG_BOARD.getShortHash(0);
	//std::cout << "hash of board not using hashtable = " << a << "\n";

	//DEBUG
	ai.bestMove = mth::PentagoMove();
	ai.ISUSINGHASHTABLE = true; //true
	auto start = std::chrono::high_resolution_clock::now();
	//int soce1 = ai.minimax(mth::PentagoMove(), maxDepth, 1, -1000, 1000, board);
	ai.clearTables();
	auto minmaxTime = std::chrono::high_resolution_clock::now();
	int scoreUsing = ai.minimax2(/*mth::PentagoMove(),*/maxDepth, 1, -1000, 1000, board); // player = 1
	std::cout << "Size of killer = " << ai.hashTable.killerHeuristic.size() << "\n";
	ai.clearTables();

	//if (scoreNotUsing != scoreUsing) { //pos1.x != ai.bestMove.marblePos.x || pos1.y != ai.bestMove.marblePos.y
	//	std::cout << "Någonting gick snett med hashtablen.... DU HAR ÄNTLIGEN HITTAT FELTET!\n";
	//	std::cout << "p1 " << scoreNotUsing << " | p2 " << scoreUsing << "\n";
	//}

	auto stop = std::chrono::high_resolution_clock::now();
	if (board.marbleAt(ai.bestMove.marblePos.x, ai.bestMove.marblePos.y) != 0) {
		std::cout << "PLACING OVER EXISTING MARBLE...\n";
		std::cout << "Placing pos = " << ai.bestMove.marblePos.x << " " << ai.bestMove.marblePos.y << " | rot: " << ai.bestMove.rotation.x << " " << ai.bestMove.rotation.y << "\n";
		return;
	}
	auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(minmaxTime - start);
	auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(stop - minmaxTime);
	//SKRIV UT DE BÄSTA DRAGEN!
	std::cout << "Time to calc minmax1: " << duration1.count() << "ms\n";
	std::cout << "Time to calc minmax2: " << duration2.count() << "ms\n";
	std::cout << "testVal minmax1 = " << ai.testVal << "testVal minmax2 = " << ai.testVal2 << "\n";
	//std::cout << "Size of killer after = " << ai.hashTable.killerHeuristic.size() << "\n";
	ai.testVal = 0;
	ai.testVal2 = 0;
	board.setMarble(ai.bestMove.marblePos.x, ai.bestMove.marblePos.y, 1);
	board.rotateSubBoard(ai.bestMove.rotation.x, ai.bestMove.rotation.y);
	ai.clearTables();
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
	nn->loadNetwork("80by3_dataset2_depth3_overtrained.txt");
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
	double rotationVal = 0;
	int maxrotationValId = 0;
	std::cout << "Outputs of neural network:\n";
	std::cout << outputs << "\n";
	//for (int i = 0; i < 36; i++) {
	//	if (outputs(i, 0) > marbleVal && board.marbleAt(i % 6, i / 6) == 0) {
	//		marbleVal = outputs(i, 0);
	//		maxMarbleValId = i;
	//	}
	//}
	//
	//for (int i = 0; i < 36; i++) { // ÄR INTE DETTA HELT FEL? DEN KOLLAR JU PÅ MARBLE OUTPUTSEN???
	//	if (outputs(i, 0) > rotationVal) {
	//		rotationVal = outputs(i, 0);
	//		maxrotationValId = i;
	//	}
	//}
	for (int i = 0; i < 288; i++) {
		int xPos = i/48; //Borde vara korrekt
		int yPos = (i/8)%6;
		int rot  = i%8;
		//std::cout << " y x z = " << yPos << " " << xPos << " " << rot << "\n";
		if (outputs(i, 0) > marbleVal && board.marbleAt(xPos, yPos) == 0) {
			marbleVal = outputs(i, 0);
			maxMarbleValId = i;
		}
	}
	int xPos = maxMarbleValId / 48; //Borde vara korrekt
	int yPos = (maxMarbleValId / 8) % 6;
	int rot = maxMarbleValId % 8;
	std::cout << "made move " << maxMarbleValId << " | val = " << marbleVal << "\n";
	//Make move
	if (board.marbleAt(xPos, yPos) != 0) {
		std::cout << "Det neurala närverket är förvirrat och försökte fuska... (lägga på annan pjäs)\n";
	}
	else {
		board.setMarble(xPos, yPos, 1);
		board.rotateSubBoard(rot / 2, rot % 2 == 0 ? 1 : -1); //OKLART / Kanske fel här!
	}
}
