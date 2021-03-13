#pragma once
#include "MyMath.h"
#include "Pentago.h"
#include "Hash.h"
#include <vector>
#include <deque>
#include <list>

namespace mm{

	class Minimax {
	private:
		int points(int streak);
		int debugVal = 0;
		bool debugBool = false;
		std::list<mth::PentagoMove> calculateMoves(ptg::PentagoGame board, int depth, int player);

	public:
		Minimax();
		int evaluate(ptg::PentagoGame board, int player);
		int evaluate2(ptg::PentagoGame board, int player);
		int minimax(mth::PentagoMove boardMove, int depth, int player, int alpha, int beta, ptg::PentagoGame board);
		int minimax2(int depth, int player, int alpha, int beta, ptg::PentagoGame board);
		int maxDepth=0;
		mth::PentagoMove bestMove;
		long testVal = 0;
		long testVal2 = 0;
		int debugVal2 = 0;
		void clearTables();
		bool ISUSINGHASHTABLE = true;
		ptg::PentagoGame DEBUG_BOARD;
		int timesCalledHash = 0;
		bool grandparent = false;
		HashTable hashTable;
	};

}