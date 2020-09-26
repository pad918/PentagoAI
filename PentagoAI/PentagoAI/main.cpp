#include <iostream>
#include <stdlib.h>
#include <chrono>
#include "Pentago.h"
#include "Minimax.h" 
#include "MyMath.h"
#include "PlayGame.h"
#include "Hash.h"
//670 lines 2020-09-19
//865 lines 2020-09-26
int main() {
	srand(0);
	
	ptg::PentagoGame pentagoBoard;
	GameHandler game;
	mm::Minimax ai; ai.maxDepth = 4; ai.testVal = 0;
	game.playAgainstMinimax(pentagoBoard, 4);

	int c; std::cin >> c;
	return 0;
}