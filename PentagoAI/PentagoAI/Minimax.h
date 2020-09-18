#pragma once
#include "MyMath.h"
#include "Pentago.h"

namespace mm{

	class Minimax {
	private:
	public:
		Minimax();
		int evaluate(ptg::PentagoGame board, int player); // Should be private
		int minimax(mth::PentagoMove boardMove, int depth, int player, ptg::PentagoGame board);
	};

}