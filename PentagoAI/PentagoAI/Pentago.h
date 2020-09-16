#pragma once
#include <iostream>

namespace ptg {
	
	class PentagoSubBoard {
	private:
		
	public:
		PentagoSubBoard();
		void printBoard(); // For debugging
		void rotate(int dir);
		unsigned char marbles[9];
	};

	class PentagoGame {
	private:
		PentagoSubBoard subBoards[4];
		int maxMarblesInARow(int player, int total, int dir, int x, int y);
		int hasWon();
	public:
		PentagoGame();
		void printBoard();
		void setMarble(int x, int y, int val);
		void rotateSubBoard(int x, int y, int dir);
		void playManualGame();
	};


}
