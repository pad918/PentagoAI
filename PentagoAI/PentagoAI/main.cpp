#include <iostream>
#include <stdlib.h>
#include <chrono>
#include "Pentago.h"
#include "Minimax.h" 
#include "MyMath.h"
#include "PlayGame.h"
//670 lines 2020-09-19

int main() {
	srand(0);
	
	ptg::PentagoGame pentagoBoard;
	pentagoBoard.getHash();
	GameHandler game;
	mm::Minimax ai; ai.maxDepth = 3; ai.testVal = 0;
	//game.playAgainstMinimax(pentagoBoard, 4);

	//TEST
	ptg::PentagoSubBoard subB;
	uint32_t tot = 0;
	for (int a = 0; a < 9; a++)
		subB.marbles[a] = a % 2 + 1;
	auto start = std::chrono::high_resolution_clock::now();
	
	for (int i = 0; i < 10000000; i++) { //948ms in debug
		for (int a = 0; a < 9; a++)
			subB.marbles[a] = a % 2 +1;
		tot += subB.getHash();
	}
			//subB.setMarble(a, a % 2 + 1);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << "Time to get hash = " << duration.count() << "ms\n val = " << subB.testVal << "\n";
	//END OF TEST
	
	


	int c; std::cin >> c;
	return 0;
}