#include "Minimax.h"
#define USINGOPT1 true

inline int mm::Minimax::points(int streak)
{
	/* Subject to change
	
		|str|pts|
		| 2	| 1	|
		| 3	| 2	|
		| 4	| 3	|
		| 5	|100| 
		| 6	|120|
	*/
	if (streak >= 5) {
		return 20*streak; 
	}
	return streak - 1;

}

mm::Minimax::Minimax()
{
	//Init
}

int mm::Minimax::evaluate(ptg::PentagoGame board, int player)
{
	//This method is slow AF, should find a smarter way to do this.
	//OBS! maybe bitwise opperations could bee used ex) https://www.geeksforgeeks.org/count-set-bits-in-an-integer/ 

	int totalPoints = 0;
	debugVal++;
	//Test for vertical "rows"
	for (int x = 0; x < 6; x++) {
		int streak = 0;
		for (int y = 0; y < 6; y++) {
			if (board.marbleAt(x, y) == player) { streak++; }
			else { if (streak > 1) { totalPoints += points(streak); } streak = 0; }
		}
		if (streak > 1) {
			totalPoints += points(streak);
		}
	}
	//Test for horizontal "rows"
	for (int y = 0; y < 6; y++) {
		int streak = 0;
		for (int x = 0; x < 6; x++) {
			if (board.marbleAt(x, y) == player) { streak++; }
			else { if (streak > 1) { totalPoints += points(streak); } streak = 0; }
		}
		if (streak > 1) {
			totalPoints += points(streak);
		}
	}
	//Test for down right "rows"
	for (int i = 0; i < 9; i++) {
		int streak = 0;
		int x = 0;
		for (int y = 4 - i; y < 6 && x < 6; y++ ) {
			if (y < 6) {
				for (;y < 0;) {
					y++;
				}
			}
			x = (y - (4 - i));
			if (x >= 0 && x < 6) {
				int tmp = streak;
				streak = (board.marbleAt(x, y) == player) ? streak + 1 : 0;
				totalPoints = (tmp > 1 && streak == 0) ? totalPoints + points(tmp) : totalPoints;
			}
		}
		if (streak > 1) {
			totalPoints += points(streak);
		}
	}
	// Test for up right "rows"
	for (int i = 0; i < 9; i++) {
		int streak = 0;
		int x = 0;
		for (int y = 1 + i; y >= 0 && x < 6; y--) {
			for (; y >= 6;) {
				y--;
			}
			x = -(y - (1 + i));
			if (x >= 0 && x < 6) {
				int tmp = streak;
				streak = (board.marbleAt(x, y) == player) ? streak + 1 : 0;
				totalPoints = (tmp > 1 && streak == 0) ? totalPoints + points(tmp) : totalPoints;
			}
		}
		if (streak > 1) {
			totalPoints += points(streak);
		}
	}
	return totalPoints;
}

int mm::Minimax::minimax(mth::PentagoMove boardMove, int depth, int player, int alpha, int beta, ptg::PentagoGame board)
{
	testVal++;

	if (depth == 0 /*Or if the game is over*/) {
		//Return evaluation of board
		int player1Eval = evaluate(board, 1);
		int player2Eval = evaluate(board, 2);
		//Return differance between the player evaluations
		return (player1Eval - player2Eval);
	}
	if(player == 1){
		int maxEvaluation = -1000;
		for (int y = 0; y < 6; y++) {
			for (int x = 0; x < 6; x++) {
				//If position is empety
				if (board.marbleAt(x, y) == 0) {
					ptg::PentagoGame newBoard = board;
					ptg::PentagoGame lastBoard;
					for (int i = 0; i < 8; i++) {
						if (board.marbleAt(x, y) == 0) {
							newBoard = board;
							mth::PentagoMove move(mth::Vector2(x, y), mth::Vector2(i / 2, (i % 2 == 0) ? 1 : -1));
							if (move.marblePos.x == 4 && move.marblePos.y == 2 && move.rotation.x == 0 && move.marblePos.y == 1) {
								std::cout << "NU SKER DET!\n";
							}
							newBoard.setMarble(x, y, 1);
							newBoard.rotateSubBoard(move.rotation.x, move.rotation.y);

							if (!USINGOPT1 || !(lastBoard == newBoard)) {

								int eval = minimax(move, depth - 1, 2, alpha, beta, newBoard);
								if (maxEvaluation < eval) {
									maxEvaluation = eval;
									if (depth == maxDepth)
										bestMove = move;
									//TEST: std::cout << "Depth = " << depth << " | move = " << bestMove.marblePos.x << " " << bestMove.marblePos.y << " | rot: " << bestMove.rotation.x << " " << bestMove.rotation.y << "\n";
								}
								if (alpha < eval) {
									alpha = eval;
								}
								if (beta <= alpha) {
									break;
								}
								lastBoard = newBoard;
							}
						}
					}
				}
			}
		}
		return maxEvaluation;
	}
	else if (player == 2) {
		int minEvaluation = 1000;
		for (int y = 0; y < 6; y++) {
			for (int x = 0; x < 6; x++) {
				//If position is empety
				if (board.marbleAt(x, y) == 0) {
					ptg::PentagoGame newBoard = board;
					ptg::PentagoGame lastBoard;
					for (int i = 0; i < 8; i++) {
						if (board.marbleAt(x, y) == 0) {
							newBoard = board;
							mth::PentagoMove move(mth::Vector2(x, y), mth::Vector2(i / 2, (i % 2 == 0) ? 1 : -1));
							newBoard.setMarble(x, y, 2);
							newBoard.rotateSubBoard(move.rotation.x, move.rotation.y);
							if (!USINGOPT1 || !(lastBoard == newBoard)) {
								int eval = minimax(move, depth - 1, 1, alpha, beta, newBoard);
								if (minEvaluation > eval) {
									minEvaluation = eval;
								}
								if (beta > eval) {
									beta = eval;
								}
								if (beta <= alpha) {
									break;
								}
								lastBoard = newBoard;
							}
						}
					}
				}
			}
		}
		return minEvaluation;
	}
	return 0;
}
