#pragma once
#include "MyMath.h"
#include "Pentago.h"

namespace mm{

	class Minimax {
	private:
		int points(int streak);
		int debugVal = 0;
	public:
		Minimax();
		int evaluate(ptg::PentagoGame board, int player); // Should be private
		int minimax(mth::PentagoMove boardMove, int depth, int player, int alpha, int beta, ptg::PentagoGame board);
		int maxDepth=0;
		mth::PentagoMove bestMove;
		int testVal = 0;
	};

}