#pragma once
#include <iostream>
#include <stdint.h>
#include "MyMath.h"
#include "Hash.h"

namespace ptg {
	
	class PentagoSubBoard {
	private:
		
	public:
		int testVal = 0;
		PentagoSubBoard();
		void printBoard(); // For debugging
		void rotate(int dir);
		unsigned char marbles[9]; 
		uint32_t getHash();
	};

	class PentagoGame {
	private:
		PentagoSubBoard subBoards[4];
		int maxMarblesInARow(int player, int total, int dir, int x, int y);
		
	public:
		PentagoGame();
		void printBoard();
		void setMarble(int x, int y, int val);
		void rotateSubBoard(int x, int y, int dir);
		void rotateSubBoard(int id, int dir);
		void playManualGame();
		/*TEST VAR FOR DEBUGGING AND PROFILEING*/ int looedTimes = 0;
		int hasWon();
		int hasWonFast();
		int marbleAt(int x, int y);
		Hash128 getHash(int depth);

	friend bool operator==(const PentagoGame& l1, const PentagoGame& l2);
	};


}
