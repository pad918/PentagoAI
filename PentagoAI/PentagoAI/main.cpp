#include <iostream>
#include <stdlib.h>
#include <chrono>
#include "Pentago.h"
#include "Minimax.h"

int main() {

	
	ptg::PentagoGame game;
	//game.playManualGame();
	game.setMarble(0, 0, 1);
	game.setMarble(0, 1, 1);
	game.setMarble(0, 2, 1);
	game.setMarble(0, 4, 1);
	game.setMarble(0, 5, 1);
	game.printBoard();
	mm::Minimax max;
	auto a = max.evaluate(game, 1);
	std::cout << "\nEval = " << a << "\n";
	//Performance test
	
	//End of test
	

	//Pause window
	int c; std::cin >> c;
	return 0;
}