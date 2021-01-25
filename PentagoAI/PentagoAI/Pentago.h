#pragma once
#include <iostream>
#include <stdint.h>
#include <algorithm>
#include <iterator>
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
		bool operator==(const PentagoSubBoard &other) const
		{
			return (std::equal(std::begin(marbles), std::end(marbles), std::begin(other.marbles)));
		}
	};

	class PentagoGame {
	private:
		
		int maxMarblesInARow(int player, int total, int dir, int x, int y);
		
	public:
		PentagoSubBoard subBoards[4];
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
		uint64_t getShortHash(int depth);
		bool operator==(const PentagoGame &other) const
		{
			return (subBoards[0] == other.subBoards[0] &&
					subBoards[1] == other.subBoards[1] &&
					subBoards[2] == other.subBoards[2] &&
					subBoards[3] == other.subBoards[3] );
		}
	friend bool operator==(const PentagoGame& l1, const PentagoGame& l2);
	};


}
