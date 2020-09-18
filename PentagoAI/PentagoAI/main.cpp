#include <iostream>
#include <stdlib.h>
#include <chrono>
#include "Pentago.h"
#include "Minimax.h"
#include "MyMath.h"
#include "PlayGame.h"

int main() {
	srand(0);
	
	ptg::PentagoGame pentagoBoard;
	GameHandler game;
	//mm::Minimax ai; ai.maxDepth = 4;
	game.playAgainstMinimax(pentagoBoard, 4);
	
	/*pentagoBoard.setMarble(0, 1, 2);
	pentagoBoard.setMarble(1, 1, 2);
	pentagoBoard.setMarble(4, 1, 2);
	pentagoBoard.setMarble(3, 2, 2);
	pentagoBoard.setMarble(1, 3, 2);
	pentagoBoard.setMarble(1, 4, 2);

	pentagoBoard.setMarble(3, 0, 1);
	pentagoBoard.setMarble(4, 0, 1);
	pentagoBoard.setMarble(5, 0, 1);
	pentagoBoard.setMarble(3, 1, 1);
	pentagoBoard.setMarble(5, 1, 1);

	pentagoBoard.printBoard();
	int a = ai.minimax(mth::PentagoMove(), 4, 1, -1000, 1000, pentagoBoard);
	std::cout << "Final move = " << ai.bestMove.marblePos.x << " " << ai.bestMove.marblePos.y << " | rot: " << ai.bestMove.rotation.x << " " << ai.bestMove.rotation.y << "\n";
	*/
	
	//game.playManualGame();
	//PERFORMANCE TEST:
	/*
	auto t1 = std::chrono::high_resolution_clock::now();
	int a = max.minimax(mth::PentagoMove(), 3, 1, game);
	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	std::cout << "\n\nTime to finnish = " << duration << "ms\n-----------------------------------\n";
	std::cout << "Perfomance, one = " << a << "\n";
	std::cout << "Best move = " << max.bestMove.marblePos.x << " " << max.bestMove.marblePos.y << " | Rot = " << max.bestMove.rotation.x << " " << max.bestMove.rotation.y << "\n";
	*/

	int c; std::cin >> c;
	return 0;
}