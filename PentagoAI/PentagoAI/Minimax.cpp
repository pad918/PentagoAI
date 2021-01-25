#include "Minimax.h"
#define USINGHASHTABLE false

inline int mm::Minimax::points(int streak)
{
	/* Subject to change
	
		|str|pts|
		| 2	| 0	|
		| 3	| 1	|
		| 4	| 2	|
		| 5	|100| 
		| 6	|120|
	*/
	if (streak >= 5) {
		return 20 * streak; 
	}
	return streak - 2;

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

	if (depth == 0 || board.hasWonFast()!=0) {
		//Return evaluation of board
		int player1Eval = evaluate(board, 1);
		int player2Eval = evaluate(board, 2);
		//Return differance between the player evaluations
		return (player1Eval - player2Eval);
	}
	if(player == 1){
		int maxEvaluation = -10000;
		for (int y = 0; y < 6; y++) {
			for (int x = 0; x < 6; x++) {
				//If position is empety
				if (board.marbleAt(x, y) == 0) {
					ptg::PentagoGame newBoard = board;
					for (int i = 0; i < 8; i++) {
						if (board.marbleAt(x, y) == 0) {
							newBoard = board;
							mth::PentagoMove move(mth::Vector2(x, y), mth::Vector2(i / 2, (i % 2 == 0) ? 1 : -1));
							newBoard.setMarble(x, y, 1);
							newBoard.rotateSubBoard(move.rotation.x, move.rotation.y);

							Hash128 hash = newBoard.getHash(depth);
							uint64_t shortHash = newBoard.getShortHash(depth);

							int eval;
							if (USINGHASHTABLE && depth == 3 && hashTableMax.isInTable(hash)) {
								eval = hashTableMax.getVal(hash);

								//NY SKRÄPKOD!
								
							}
							else
							{
								eval = minimax(move, depth - 1, 2, alpha, beta, newBoard); 
								if (USINGHASHTABLE && depth == 3) {
									hashTableMax.addElement(hash, eval);

									//NY SKRÄPKOD! TEST
									
								}
							}

							if (maxEvaluation < eval) {
								maxEvaluation = eval;
								if (depth == maxDepth) {
									bestMove = move;
									//std::cout << "Found best move: " << eval << " | hash = " << hash.val[0] << " " << hash.val[1] << "\n";
								}
								
							}
							alpha = (eval > alpha) ? eval : alpha;
							if (beta <= alpha) {
								break;
							}	
						}
					}
				}
			}
		}
		return maxEvaluation;
	}
	else if (player == 2) {
		int minEvaluation = 10000;
		for (int y = 0; y < 6; y++) {
			for (int x = 0; x < 6; x++) {
				//If position is empety
				if (board.marbleAt(x, y) == 0) {
					ptg::PentagoGame newBoard = board;
					for (int i = 0; i < 8; i++) {
						if (board.marbleAt(x, y) == 0) {
							newBoard = board;
							mth::PentagoMove move(mth::Vector2(x, y), mth::Vector2(i / 2, (i % 2 == 0) ? 1 : -1));
							newBoard.setMarble(x, y, 2);
							newBoard.rotateSubBoard(move.rotation.x, move.rotation.y);
						
							Hash128 hash = newBoard.getHash(depth); // depth
							//uint64_t shortHash = newBoard.getShortHash(depth);

							int eval;
							if (USINGHASHTABLE && depth == 3 && hashTableMin.isInTable(hash)) { // ATT GÖRA: mindre ska kunna använda större...
								eval = hashTableMin.getVal(hash);

								//Ny kod
								//eval = hashTableMin.getVal(shortHash);
							}
							else
							{
								eval = minimax(move, depth - 1, 1, alpha, beta, newBoard);
								if (USINGHASHTABLE && depth == 3) {
									hashTableMin.addElement(hash, eval);

									//NY SKRÄPKOD! 
									//hashTableMin.addElement(shortHash, eval);
								}
							}
							if (minEvaluation > eval) {
								minEvaluation = eval;
							}
							beta = (eval < beta) ? eval : beta;
							if (beta <= alpha) {
								break;
							}
							
						}
					}
				}
			}
		}
		return minEvaluation;
	}
	return 1337; // To be removed
}

void mm::Minimax::clearTables()
{
	hashTableMax.hashList.clear();
	hashTableMin.hashList.clear();
	hashTableMax.valueList.clear();
	hashTableMin.valueList.clear();
	//hashTableMax.clear();
	//hashTableMin.clear();
}
