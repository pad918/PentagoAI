#include "PlayGame.h"
#include <chrono>

GameHandler::GameHandler()
{
}

void GameHandler::playAgainstMinimax(ptg::PentagoGame &board, int depth)
{
	mm::Minimax ai;
	ai.maxDepth = depth;
	int hw = 0;
	int aiPoints = 0;
	int plTurn = 0;
	for (int i = 0; i < 18; i++) {
		//Player move
		std::cout << "\n";
		//system("cls"); //REMOVE SHITTY WINDOWS ONLY SHIT!
		board.printBoard();
		std::cout << "\n \n AI BUFFER: " << (ai.hashTableMax.hashList.size() + ai.hashTableMin.hashList.size()) << "\n\n";
		int x, y;
		std::cout << "Player's turn. " << "| Set marble: x:y : ";
		std::cin >> x >> y;
		board.setMarble(x, y, 2);
		hw = board.hasWonFast();
		if (hw != 0) {
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
		hw = board.hasWonFast();
		if (hw != 0) {
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
		hw = board.hasWonFast();
		if (hw != 0) {
			break;
		}
	}
	
	std::cout << "\n\n";
	//system("cls");
	board.printBoard();
	std::cout << "  Player: " << hw << " WON!\n";
}
