#include <iostream>
#include <stdlib.h>
#include <chrono>
#include "Pentago.h"

int main() {


	ptg::PentagoGame game;
	//game.playManualGame();

	
	//Performance test
	srand(0); //0
	for (int i = 0; i < 30; i++) {
		game.setMarble(rand() % 6, rand()% 6, rand() % 2 + 1);
	}
	
	auto t1 = std::chrono::high_resolution_clock::now();
	
	for(int i=0; i<10000; i++)
		game.hasWonFast(); // ca 211 tests, srand = 0 ... 10 000 times in 137ms in debugg mode and 14ms in release mode.
	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	game.printBoard();
	std::cout << "\nTotal tests = " << game.looedTimes << " | Time = " << duration << "ms\n";
	
	//End of test
	

	//Pause window
	int c; std::cin >> c;
	return 0;
}