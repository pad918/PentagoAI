#include "Minimax.h"

mm::Minimax::Minimax()
{
	//Init
}

int mm::Minimax::evaluate(ptg::PentagoGame board, int player)
{
	//This method is slow AF, should find a smarter way to do this.
	//OBS! maybe bitwise opperations could bee used ex) https://www.geeksforgeeks.org/count-set-bits-in-an-integer/ 

	/* To be changed
		points += round down of (streak ^ 1.5)
	*/
	int totalPoints = 0;
	
	//Test for vertical "rows"
	for (int x = 0; x < 6; x++) {
		int streak = 0;
		for (int y = 0; y < 6; y++) {
			if (board.marbleAt(x, y) == player) { streak++; }
			else { if (streak > 1) { totalPoints += (int)(pow(streak, 1.5f)); } streak = 0; }
		}
		if (streak > 1) {
			totalPoints += (int)(pow(streak, 1.5f));
		}
	}
	//Test for horizontal "rows"
	for (int y = 0; y < 6; y++) {
		int streak = 0;
		for (int x = 0; x < 6; x++) {
			if (board.marbleAt(x, y) == player) { streak++; }
			else { if (streak > 1) { totalPoints += (int)(pow(streak, 1.5f)); } streak = 0; }
		}
		if (streak > 1) {
			totalPoints += (int)(pow(streak, 1.5f));
		}
	}
	//Test for down left "rows"
	


	return totalPoints;
}

int mm::Minimax::minimax(mth::PentagoMove boardMove, int depth, int player, ptg::PentagoGame board)
{

	if (depth == 0 /*Or if thee game is over*/) {
		//Return evaluation of board
		int player1Eval = 0;
		int player2Eval = 0;
		//Return differance between the player evaluations
		return /*Eval*/0;
	}
	if(player == 1){
		int maxEvaluation = -INFINITY;
		for (int y = 0; y < 6; y++) {
			for (int x = 0; x < 6; x++) {
				//If position is empety
				if (board.marbleAt(x, y) == 0) {
					for (int i = 0; i < 8; i++) {
						ptg::PentagoGame newBoard= board;
						newBoard.setMarble(x, y, 1);
						newBoard.rotateSubBoard(i/2, i%2-1);
						int eval = minimax(mth::PentagoMove(mth::Vector2(x, y), mth::Vector2(i/2,i%2-1)), depth-1, 2, newBoard);
						maxEvaluation = (maxEvaluation < eval) ? eval : maxEvaluation;
					}
				}
			}
		}
	}
	else if (player == 2) {
		int maxEvaluation = -INFINITY;
		for (int y = 0; y < 6; y++) {
			for (int x = 0; x < 6; x++) {
				//If position is empety
				if (board.marbleAt(x, y) == 0) {
					for (int i = 0; i < 8; i++) {
						ptg::PentagoGame newBoard = board;
						newBoard.setMarble(x, y, 1);
						newBoard.rotateSubBoard(i / 2, i % 2 - 1);
						int eval = minimax(mth::PentagoMove(mth::Vector2(x, y), mth::Vector2(i / 2, i % 2 - 1)), depth - 1, 1, newBoard);
						maxEvaluation = (maxEvaluation > eval) ? eval : maxEvaluation;
					}
				}
			}
		}
	}
	return 0;
}
