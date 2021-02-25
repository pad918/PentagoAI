#include <iostream>
#include <stdlib.h>
#include <chrono>
#include "Pentago.h"
#include "Minimax.h" 
#include "MyMath.h"
#include "PlayGame.h"
#include "Hash.h"
#include "NeuralNetwork.h"
#include "NNTrainer.h"
//670  lines 2020-09-19
//865  lines 2020-09-26
//1284 lines 2021-01-13
//1745 lines 2021-02-01

int main() {
	
	/*
	srand(time(NULL));
	NNTrainer tester;
	tester.trainAgainstMinmax();
	*/

	//DEBUGING!
	//ptg::PentagoGame board;
	//board.loadBoardFromHash(15281);
	//board.printBoard();
	//mm::Minimax ai;
	//ai.maxDepth = 3;
	//uint64_t hash = 15281;
	//int tst = 0;
	//for (int i = 0; i < 1; ++i) {
	//	//hash = rand() % 100000000000;
	//	board.loadBoardFromHash(hash);
	//	ai.clearTables();
	//	ai.ISUSINGHASHTABLE = true;
	//	auto val1 = ai.minimax(mth::PentagoMove(), 3, 1, -1000, 1000, board);
	//	std::cout << "Times called hash = " << ai.timesCalledHash << "\n";
	//	ai.timesCalledHash = 0;
	//	std::cout << "******************************************************\n";
	//	auto hash1 = ai.DEBUG_BOARD.getShortHash(0);
	//	ai.ISUSINGHASHTABLE = false;
	//	auto val2 = ai.minimax(mth::PentagoMove(), 3, 1, -1000, 1000, board);
	//	std::cout << "Times called hash = " << ai.timesCalledHash << "\n";
	//	ai.timesCalledHash = 0;
	//	auto hash2 = ai.DEBUG_BOARD.getShortHash(0);
	//	if (val1 != val2) {
	//		std::cout << "HASHVALUE = " << hash << "\n";
	//		std::cout << "hash of best = " << hash1 << " | " << hash2 << "\n";
	//		std::cout << "Val 1 = " << val1 << " | val 2 = " << val2 << "\n";
	//	}
	//	else {
	//		std::cout << ".";
	//		if (++tst > 50) {
	//			tst = 0;
	//			std::cout << "\n";
	//		}
	//	}
	//}
	//END OF DEBUG

	srand(0);
	GameHandler game;
	game.playGame(0, 2);
	
	
	//stop program from stopping
	int c; std::cin >> c;
	return 0;
}