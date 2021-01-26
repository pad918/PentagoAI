#pragma once
#include "MyMath.h"
#include "Pentago.h"
#include "Hash.h"

namespace mm{

	class Minimax {
	private:
		int points(int streak);
		int debugVal = 0;
		bool debugBool = false;
		
	public:
		Minimax();
		int evaluate(ptg::PentagoGame board, int player); // Should be private
		int minimax(mth::PentagoMove boardMove, int depth, int player, int alpha, int beta, ptg::PentagoGame board);
		int maxDepth=0;
		mth::PentagoMove bestMove;
		int testVal = 0;
		int debugVal2 = 0;
		void clearTables();
		HashTable hashTable;
	};

}