#include "Minimax.h"
#define USINGHASHTABLE true

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

inline std::list<mth::PentagoMove> mm::Minimax::calculateMoves(ptg::PentagoGame board, int depth, int player)
{
	std::list<mth::PentagoMove> moves;
	for (int y = 0; y < 6; y++) {
		for (int x = 0; x < 6; x++) {
			if (board.marbleAt(x, y) != 0) {
				continue;
			}
			for (int i = 0; i < 8; i++) {
				auto move = mth::PentagoMove(mth::Vector2(x, y), mth::Vector2(i / 2, (i % 2 == 0) ? 1 : -1));
				moves.push_back(move);
				//if (depth > 2) {
				//	uint64_t key = move.getHash() + depth << 20;
				//	if (hashTable.killerHeuristic.find(key) != hashTable.killerHeuristic.end()) {
				//		moves.push_front(move);
				//	}
				//	else {
				//		moves.push_back(move);
				//	}
				//}
				//else {
				//	moves.push_back(move);
				//}
			}
		}
	}
	return moves;
}

mm::Minimax::Minimax()
{
	//Init
	debugBool = USINGHASHTABLE;
}

int mm::Minimax::evaluate(ptg::PentagoGame board, int player)
{
	//This method is slow AF, should find a smarter way to do this.

	int totalPoints = 0;
	debugVal++;
	//Test for vertical "rows"
	for (int x = 0; x < 6; x++) {
		int streak = 0;
		for (int y = 0; y < 6; y++) {
			if (board.subBoards[(y > 2) * 2 + (x > 2) * 1].marbles[3 * (y - (y > 2) * 3) + (x - (x > 2) * 3)] == player) { streak++; }
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
			if (board.subBoards[(y > 2) * 2 + (x > 2) * 1].marbles[3 * (y - (y > 2) * 3) + (x - (x > 2) * 3)] == player) { streak++; }
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
				streak = (board.subBoards[(y > 2) * 2 + (x > 2) * 1].marbles[3 * (y - (y > 2) * 3) + (x - (x > 2) * 3)] == player) ? streak + 1 : 0;
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
				streak = (board.subBoards[(y > 2) * 2 + (x > 2) * 1].marbles[3 * (y - (y > 2) * 3) + (x - (x > 2) * 3)] == player) ? streak + 1 : 0;
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

							uint64_t shortHash = newBoard.getShortHash(0);

							int eval;
							if (ISUSINGHASHTABLE && depth == hashTable.highestDepthOfHashNy(shortHash)) { // <=
								eval = hashTable.getValNy(shortHash);
							}
							else
							{

								eval = minimax(move, depth - 1, 2, alpha, beta, newBoard); 
								if (ISUSINGHASHTABLE && depth == maxDepth) { //only add highest depth
									hashTable.addElementNy(shortHash, eval, depth);

								}
							}

							if (maxEvaluation < eval) {
								maxEvaluation = eval;
								if (depth == maxDepth) {
									bestMove = move;
									DEBUG_BOARD = newBoard;
									//std::cout << "Found best move: " << eval << " | hash = " << hash.val[0] << " " << hash.val[1] << "\n";
								}
								
							}
							alpha = (eval > alpha) ? eval : alpha;
							if (beta <= alpha) {
								return maxEvaluation;
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

							uint64_t shortHash = newBoard.getShortHash(0);

							int eval;
							if (ISUSINGHASHTABLE && depth == hashTable.highestDepthOfHashNy(shortHash)) { // <=

								eval = hashTable.getValNy(shortHash);

							}
							else
							{
								eval = minimax(move, depth - 1, 1, alpha, beta, newBoard);
								if (ISUSINGHASHTABLE && depth == maxDepth) { //only add highest depth

									hashTable.addElementNy(shortHash, eval, depth);
								}
							}

							if (minEvaluation > eval) {
								minEvaluation = eval;
							}
							beta = (eval < beta) ? eval : beta;
							if (beta <= alpha) { // ska det vara <= eller <
								return minEvaluation;
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

int mm::Minimax::minimax2(int depth, int player, int alpha, int beta, ptg::PentagoGame board)
{
	testVal2++;
	if (depth == 0 || board.hasWonFast() != 0) {
		//Return evaluation of board
		int player1Eval = evaluate(board, 1);
		int player2Eval = evaluate(board, 2);
		//Return differance between the player evaluations
		int eval = (player==2) ? player2Eval - player1Eval : player1Eval - player2Eval;
		return eval;
	}

	auto key = board.getShortHash(0);

	//Hashtabele kanske fungerar; vem vet?
	if (hashTable.hashMap.find(key) != hashTable.hashMap.end() && hashTable.highestDepthOfHashNy(key) >= depth) {
		auto hashf = hashTable.hashf[key];
		switch (hashf) {
		case hashfEXACT:
			return hashTable.hashMap[key];
			break;
		case hashfALPHA:
			if (hashTable.hashMap[key] <= alpha) {
				return alpha;
			}
			break;
		case hashfBETA:
			if (hashTable.hashMap[key] >= beta) {
				return beta;
			}
			break;
		}
	}

	int hashf = hashfALPHA;

	auto legalMoves = calculateMoves(board, depth, player);

	for (auto &move : legalMoves) {

		//Make move on a new board
		ptg::PentagoGame newBoard = board;
		newBoard.setMarble(move.marblePos.x, move.marblePos.y, player);
		newBoard.rotateSubBoard(move.rotation.x, move.rotation.y);
		
		//BLABLA
		//uint64_t shortHash = newBoard.getShortHash(0);
		int eval;

		eval = -minimax2(depth - 1, (player == 2) ? 1 : 2, -beta, -alpha, newBoard);

		//Find max or min depending on player
		if (eval > alpha) {
			alpha = eval;
			hashf = hashfEXACT;
			if (depth == maxDepth) {
				//std::cout << "found best move\n";
				bestMove = move;
			}
		}

		if (beta <= alpha) {
			
			//SAVE HASH HERE:
			hashTable.addElementNy(key, alpha, depth); //OKLART!
			hashTable.hashf[key] = hashfBETA; //NY DUM KOD?

			return beta;
		}
	}
	//Save hash;
	
	hashTable.addElementNy(key, alpha, depth); //OKLART!
	hashTable.hashf[key] = hashf;
	return alpha; 
}

void mm::Minimax::clearTables()
{
	hashTable.hashMap.clear();
	hashTable.killerHeuristic.clear();
	hashTable.highestValuesMap.clear();
	hashTable.hashf.clear();
}
