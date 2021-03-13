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

	//Välja djup själv + visa tiden för varje spelare.

	//User interface
	std::cout << "--------------------------------\nplayer         = 0\nminimax        = 1\nneural network = 2\n------------------------------" << std::endl;
	for (int i = 1; i <= 2; i++) {
		std::cout << "Player " << ((i==1) ? "one" : "two") << " = ";
		int playerType = 0;
		std::cin >> playerType;
		int depthOfMinimax = 0;
		switch (playerType)
		{
		case 0:
			//if (i == 1) { playerOne = new HumanPlayer; playerOne->playerVal = 1; }
			//else		{ playerTwo = new HumanPlayer; playerTwo->playerVal = 2; }
			players[i - 1] = new HumanPlayer; 
			players[i - 1]->playerVal = i;
			break;
		case 1:
			std::cout << "Minimax depth: ";
			std::cin >> depthOfMinimax;
			//if (i == 1) { playerOne = new MinimaxPlayer; playerOne->maxDepth = depthOfMinimax; playerOne->playerVal = 1; }
			//else		{ playerTwo = new MinimaxPlayer; playerTwo->maxDepth = depthOfMinimax; playerTwo->playerVal = 2; }
			players[i - 1] = new MinimaxPlayer;
			players[i - 1]->maxDepth = depthOfMinimax;
			players[i - 1]->playerVal = i;
			break;
		case 2:
			//if (i == 1) { playerOne = new NNPlayer; playerOne->playerVal = 1; }
			//else		{ playerTwo = new NNPlayer; playerTwo->playerVal = 2; }
			players[i - 1] = new NNPlayer; 
			players[i - 1]->playerVal = i;
			break;
		default:
			std::cout << "Illegal value, restart program...\n";
			return;
		}
	}

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

		if (gameOver) {
			continue;
		}

		int pollingPlayer = 2 * players[1]->isPollingInput + players[0]->isPollingInput;
		if (pollingPlayer>0) {
			//Do the polling
			if (mouseReleasedThisFrame) {
				if (gameGUI.poll(*window, board)) {
					if (gameGUI.wasMoveLegal) {
						board.rotateSubBoard(gameGUI.rotateBoard.x, gameGUI.rotateBoard.y, gameGUI.rotateBoard.z); //ändra
						players[0]->isPollingInput = false;
						players[1]->isPollingInput = false;
					}
				}
				else {
					if (!gameGUI.wasMoveLegal) {
						std::cout << "Move is illegal\n";
					}
					else {
						if (pollingPlayer == 1)
							board.setMarble(gameGUI.setMarblePos.x, gameGUI.setMarblePos.y, 1);
						else
							board.setMarble(gameGUI.setMarblePos.x, gameGUI.setMarblePos.y, 2);
					}
				}
				hasWon = board.hasWonFast();
				if (hasWon != 0) {
					std::cout << "player " << hasWon << "Won!" << std::endl;
					gameOver = true;
				}
			}
		}
		else {
			switch (frameCounter++ % 2)
			{
			case 0:
				//Player two's move
				std::cout << "\n";
				players[0]->doMove(board);
				hasWon = board.hasWonFast();
				if (hasWon != 0) {
					std::cout << "player " << hasWon << "Won!" << std::endl;
					gameOver = true;
				}
				break;
			case 1:
				//Player one's turn
				std::cout << "\n";
				players[1]->doMove(board);
				hasWon = board.hasWonFast();
				if (hasWon != 0) {
					std::cout << "player " << hasWon << "Won!" << std::endl;
					gameOver = true;
				}
				break;
			}
		}

		window->clear();
		gameGUI.update(*window, board);
		window->display();
	
	}
	std::cout << "Spelet avslutades av okännd anledning\n";

}

void HumanPlayer::doMove(ptg::PentagoGame & board)
{
	isPollingInput = true;
}

void MinimaxPlayer::doMove(ptg::PentagoGame & board)
{
	ai.maxDepth = maxDepth; // borde flytta till där den inizieras;
	std::cout << "\nminimax AI's turn...\n";
	ai.bestMove = mth::PentagoMove();
	ai.ISUSINGHASHTABLE = true; 
	auto start = std::chrono::high_resolution_clock::now();
	int scoreUsing = ai.minimax2(maxDepth, playerVal, -1000, 1000, board); 
	ai.clearTables();

	//Print error if minimax was cheating.
	auto stop = std::chrono::high_resolution_clock::now();
	if (board.marbleAt(ai.bestMove.marblePos.x, ai.bestMove.marblePos.y) != 0) {
		std::cout << "PLACING OVER EXISTING MARBLE...\n";
		std::cout << "Placing pos = " << ai.bestMove.marblePos.x << " " << ai.bestMove.marblePos.y << " | rot: " << ai.bestMove.rotation.x << " " << ai.bestMove.rotation.y << "\n";
		return;
	}
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << "Time to calc minmax: " << duration.count() << "ms\n";

	board.setMarble(ai.bestMove.marblePos.x, ai.bestMove.marblePos.y, playerVal);
	board.rotateSubBoard(ai.bestMove.rotation.x, ai.bestMove.rotation.y);
	ai.clearTables();
}

NNPlayer::NNPlayer()
{
	std::vector<int> layerSizes; 
	layerSizes.push_back(10);
	//layerSizes.push_back(10);
	for(int h=0; h<2; h++)
		layerSizes.push_back(10);
	

	layerSizes.push_back(10);
	nn = new NeuralNetwork(layerSizes);
	nn->loadNetwork("150by3_tanh_lr2.txt");
}

void NNPlayer::doMove(ptg::PentagoGame & board)
{
	std::cout << "\n";
	std::cout << "Neural network is choosing its move...\n";
	auto start = std::chrono::high_resolution_clock::now();
	//Set inputs
	Eigen::MatrixXd inputs(108, 1);
	inputs.setZero();
	for (int j = 0; j < 36; j++) {
		int player = board.marbleAt(j % 6, j / 6);
		
		//Training data assumed neural network to be player one
		if (playerVal == 2) {
			player = (player > 0) ? (player==2) ? 1 : 2 : 0;
		}
		inputs(j * 3 + player, 0) = 1;
	}
	nn->setInputs(inputs);

	//Generate outputs
	auto outputs = nn->calculateOutputs();
	double marbleVal=0;
	int maxMarbleValId=0;
	double rotationVal = 0;
	int maxrotationValId = 0;
	//std::cout << "Outputs of neural network:\n";
	//std::cout << outputs << "\n";
	for (int i = 0; i < 288; i++) {
		int yPos = i/48; //Borde vara korrekt
		int xPos = (i/8)%6;
		int rot  = i%8;
		//std::cout << " y x z = " << yPos << " " << xPos << " " << rot << "\n";
		if (outputs(i, 0) > marbleVal && board.marbleAt(xPos, yPos) == 0) {
			marbleVal = outputs(i, 0);
			maxMarbleValId = i;
		}
	}
	int yPos = maxMarbleValId / 48; //Borde vara korrekt
	int xPos = (maxMarbleValId / 8) % 6;
	int rot = maxMarbleValId % 8;
	auto end = std::chrono::high_resolution_clock::now();
	//std::cout << "made move " << maxMarbleValId << " | val = " << marbleVal << "\n";
	//Make move
	if (board.marbleAt(xPos, yPos) != 0) {
		std::cout << "Det neurala närverket är förvirrat och försökte fuska... (lägga på annan pjäs)\n";
	}
	else {
		board.setMarble(xPos, yPos, playerVal);
		board.rotateSubBoard(rot / 2, rot % 2 == 0 ? 1 : -1); //OKLART / Kanske fel här!
	}
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Time to calc NN: " << duration.count() << "ms\n";
}
