#include "Pentago.h"

ptg::PentagoSubBoard::PentagoSubBoard()
{
	//Set all marbles poitions to zero, meaning no marble
	memset(marbles,		0, sizeof(unsigned char) * 9);
}

void ptg::PentagoSubBoard::printBoard()
{
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			std::cout << (int)marbles[y * 3 + x] << " ";
		}
		std::cout << "\n";
	}
}

void ptg::PentagoSubBoard::rotate(int dir)
{
	/*
	dir =  1 ==> medsols
	dir = -1 ==> motsols
	*/
	//Safety check
	if (std::abs(dir) != 1) { std::cout << "Warning: rotaion direction invalid\n"; return; }
	unsigned char tmp[9];
	if (dir == 1) {
		for (int i = 0; i < 9; i++)
			tmp[i] = marbles[i];
		marbles[0] = tmp[6];
		marbles[1] = tmp[3];
		marbles[2] = tmp[0];
		marbles[3] = tmp[7];
		marbles[5] = tmp[1];
		marbles[6] = tmp[8];
		marbles[7] = tmp[5];
		marbles[8] = tmp[2];
	}
	else if (dir == -1) {
		for (int i = 0; i < 9; i++)
			tmp[i] = marbles[i];
		marbles[0] = tmp[2];
		marbles[1] = tmp[5];
		marbles[2] = tmp[8];
		marbles[3] = tmp[1];
		marbles[5] = tmp[7];
		marbles[6] = tmp[0];
		marbles[7] = tmp[3];
		marbles[8] = tmp[6];
	}

}



uint32_t ptg::PentagoSubBoard::getHash()
{
	uint32_t hash = 0;
	for (int i = 0; i < 9; i++) {
		switch (marbles[i])
		{
		case 1:
			hash |= 1UL << (2*i);
			testVal++;
			break;
		case 2:
			hash |= 1UL << (2 * i + 1);
			testVal--;
			break;
		}
	}
	return hash;
}

int ptg::PentagoGame::maxMarblesInARow(int player, int total, int dir, int x, int y)
{
	//Recursive function
	/*
		There are 8 dirctions, 
		0 = up right
		1 = right
		2 = down right
		3 = down
		4 = down left
		5 = left
		6 = up left
		7 = up
	*/
	looedTimes++;
	int max = 0;
	int dirs[8]; 
	if (dir < 0) {
		for(int i=0; i<8; i++)
			dirs[i] = maxMarblesInARow(player, 0, i, x, y);
		for (int i = 0; i < 8; i++) {
			if (max < dirs[i]) { max = dirs[i]; }
		}
	}
	else {
		int subBoardPos = 0;
		int subBoardXPos = 0;
		int subBoardYPos = 0;
		switch (dir)
		{
			case 0:
				x++;
				y--;

				break;
			case 1:
				x++;
				break;
			case 2:
				x++;
				y++;
				break;
			case 3:
				y++;
				break;
			case 4:
				y++;
				x--;
				break;
			case 5:
				x--;
				break;
			case 6:
				y--;
				x--;
				break;
			case 7:
				y--;
				break;
			default:
				return total;
				break;
		}

		subBoardPos = (y / 3) * 2 + (x / 3) * 1;
		subBoardXPos = (x % 3);
		subBoardYPos = (y % 3);
		if (subBoards[subBoardPos].marbles[subBoardYPos * 3 + subBoardXPos] == player && (x < 6 && x >= 0 && y < 6 && y >= 0)) { // && (x<6 && x>=0 && y<6 && y>=0)
			total = maxMarblesInARow(player, total + 1, dir, x, y);

		}
		else {
			return total;
		}

	}
	if (total > max) { max = total; }
	return max;
}

int ptg::PentagoGame::hasWon()
{
	//Test player one:
	looedTimes = 0;
	int playerOneMaxInARow = 0; 1 + maxMarblesInARow(1, 0, -1, 0, 0);
	int playerTwoMaxInARow = 0; 1 + maxMarblesInARow(2, 0, -1, 0, 0);
	for (int y = 0; y < 6; y++) {
		int localY = y % 3;
		for (int x = 0; x < 6; x++) {
			int localX = x % 3;
			int subBoardPos = (y/3)*2+(x/3);
			if (subBoards[subBoardPos].marbles[y * 3 + x] == 1) {
				int maxInRow = 1 + maxMarblesInARow(1, 0, -1, x, y);
				playerOneMaxInARow = (playerOneMaxInARow < maxInRow) ? maxInRow : playerOneMaxInARow;
			}
			if (subBoards[subBoardPos].marbles[y * 3 + x] == 2) {
				int maxInRow = 1 + maxMarblesInARow(2, 0, -1, x, y);
				playerTwoMaxInARow = (playerTwoMaxInARow < maxInRow) ? maxInRow : playerTwoMaxInARow;
			}
		}
	}
	if (playerOneMaxInARow >= 5) { return 1; }
	if (playerTwoMaxInARow >= 5) { return 2; }
	return 0;
}

int ptg::PentagoGame::hasWonFast()
{
	looedTimes = 0;
	for (int y = 0; y < 6; y++) {
		for (int x = 0; x < 6; x++) {
			int localY = y % 3;
			int localX = x % 3;
			int subBoardPos = (y / 3) * 2 + (x / 3);
			int player = subBoards[subBoardPos].marbles[localY * 3 + localX];
			if (player != 0) {
				looedTimes++;
				//Up and down test
				if (y < 2) {
					localY = (y + 4) % 3;
					localX = x % 3;
					subBoardPos = ((y + 4) / 3) * 2 + (x / 3);
					if (player == subBoards[subBoardPos].marbles[localY * 3 + localX]){
						//Testa alla i mitten...
						bool knowItIsNot5 = false;
						for (int i = 1; i < 4; i++) {
							localY = (y+i) % 3;
							subBoardPos = ((y + i) / 3) * 2 + (x / 3);
							if (subBoards[subBoardPos].marbles[localY * 3 + localX] != player) { knowItIsNot5 = true; }
						}
						if (!knowItIsNot5) { return player; }
					}
				}
				//Right and left test
				if (x < 2) {
					localY = y % 3;
					localX = (x + 4) % 3;
					subBoardPos = ((y) / 3) * 2 + ((x + 4) / 3);
					if (player == subBoards[subBoardPos].marbles[localY * 3 + localX]) {
						//Testa alla i mitten...
						bool knowItIsNot5 = false;
						for (int i = 1; i < 4; i++) {
							localX = (x + i) % 3;
							subBoardPos = (y / 3) * 2 + ((x+i) / 3);
							if (subBoards[subBoardPos].marbles[localY * 3 + localX] != player) { knowItIsNot5 = true; }
						}
						if (!knowItIsNot5) { return player; }
					}
				}
				//Down left test
				if (x < 2 && y < 2) {
					localY = (y + 4) % 3;
					localX = (x + 4) % 3;
					subBoardPos = ((y + 4) / 3) * 2 + ((x + 4) / 3);
					if (player == subBoards[subBoardPos].marbles[localY * 3 + localX]) {
						//Testa alla i mitten...
						bool knowItIsNot5 = false;
						for (int i = 1; i < 4; i++) {
							localX = (x + i) % 3;
							localY = (y + i) % 3;
							subBoardPos = ((y + i) / 3) * 2 + ((x + i) / 3);
							if (subBoards[subBoardPos].marbles[localY * 3 + localX] != player) { knowItIsNot5 = true; }
						}
						if (!knowItIsNot5) { return player; }
					}
				}
				//up right test
				if (x < 2 && y >= 4) {
					localY = (y - 4) % 3;
					localX = (x + 4) % 3;
					subBoardPos = ((y - 4) / 3) * 2 + ((x + 4) / 3);
					if (player == subBoards[subBoardPos].marbles[localY * 3 + localX]) {
						//Testa alla i mitten...
						bool knowItIsNot5 = false;
						for (int i = 1; i < 4; i++) {
							localX = (x + i) % 3;
							localY = (y - i) % 3;
							subBoardPos = ((y - i) / 3) * 2 + ((x + i) / 3);
							if (subBoards[subBoardPos].marbles[localY * 3 + localX] != player) { knowItIsNot5 = true; }
						}
						if (!knowItIsNot5) { return player; }
					}
				}
			}
		}
	}

	return 0;
}

int ptg::PentagoGame::marbleAt(int x, int y)
{
	//Converts abs pos to relative (cleans code)

	//OLD SLOW
	//int subBoardYPos = (y % 3);
	//int subBoardPos = (y / 3) * 2 + (x / 3) * 1;
	//int subBoardXPos = (x % 3);
	//return subBoards[subBoardPos].marbles[3*subBoardYPos+subBoardXPos];

	//New fast?
	int subBoardYPos = y - (y > 2) * 3;
	int subBoardPos = (y > 2) * 2 + (x > 2) * 1;
	int subBoardXPos = x - (x > 2) * 3;
	return subBoards[subBoardPos].marbles[3 * subBoardYPos + subBoardXPos];
}

uint64_t ptg::PentagoGame::getShortHash(int depth)
{
	uint64_t hash = 0;
	uint64_t mul = 1;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 9; j++) {
			/*mul *= 3;*/
			hash += subBoards[i].marbles[j] * mul;
			mul *= 3;
		}
	}
	hash += depth << 60;
	return hash;
}

void ptg::PentagoGame::loadBoardFromHash(uint64_t hash)
{
	uint64_t mul = 1;
	for (int i = 0; i < 35; i++) {
		mul *= 3;
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 9; j++) {
			uint64_t temp = hash;
			temp /= (mul!=0) ? mul : 1;
			temp = temp & 0b11;
			hash -= mul * temp;
			subBoards[3-i].marbles[8-j] = (temp>2) ? 2 : temp;
			mul /= 3;
		}
	}
}

Hash128 ptg::PentagoGame::getHash(int depth)
{
	uint64_t bigWord   = ((uint64_t)subBoards[0].getHash() << 32) + subBoards[1].getHash();
	bigWord += (uint64_t)depth << 60; //borde fungera
	uint64_t smallWord = ((uint64_t)subBoards[2].getHash() << 32) + subBoards[3].getHash();
	return Hash128(bigWord, smallWord);
}

ptg::PentagoGame::PentagoGame()
{

}

void ptg::PentagoGame::printBoard()
{
	std::cout << "Board: \n\n";
	for (int y = 0; y < 6; y++) {
		int subBoardYPos = (y % 3);
		if (subBoardYPos == 0) { std::cout << "\n"; }
		for (int x = 0; x < 6; x++) {
			int pos = (y / 3)*2 + (x/3)*1;
			int subBoardXPos = (x % 3);
			
			if (subBoardXPos == 0) { std::cout << " "; }
			std::cout << (int)subBoards[pos].marbles[subBoardYPos*3 + subBoardXPos] << " ";
		}
		
		std::cout << "\n";
	}
}

void ptg::PentagoGame::setMarble(int x, int y, int val)
{
	//Error checking
	if (x < 0 || x>5 || y < 0 || y>5) { std::cout << "Warning: Incorrect board position accessed in doMove()\n"; return; }
	if (!(val == 1 || val == 2)) { std::cout << "Value incorrect in doMove\n"; return; }

	int boardPos = (x / 3) + (y / 3) * 2;
	int subBoardXPos = x % 3;
	int subBoardYPos = y % 3;
	subBoards[boardPos].marbles[subBoardXPos + subBoardYPos * 3] = val;

}

void ptg::PentagoGame::rotateSubBoard(int x, int y, int dir)
{
	if (x < 0 || x>1 || y < 0 || y>1) { std::cout << "incorrect board position in rotateSubBoard()\n"; return; }
	subBoards[y * 2 + x].rotate(dir);
}

void ptg::PentagoGame::rotateSubBoard(int id, int dir)
{
	if (id < 0 || id>3 || dir > 1 || dir < -1 || dir == 0) { std::cout << "Incorrect board rotation in rotateSubBoard()\n"; }
	subBoards[id].rotate(dir);
}

void ptg::PentagoGame::playManualGame()
{
	int hw=0;
	int plTurn = 0;
	for (int i = 0; i < 36; i++) {
		system("cls"); //REMOVE, WINDOWS ONLY!
		printBoard();
		std::cout << "\n";
		int x, y;
		std::cout << "Player " << plTurn+1  << "'s turn. " << "Set marble: x:y : ";
		std::cin >> x >> y;
		setMarble(x, y, plTurn+1);
		hw = hasWonFast();
		if (hw != 0) {
			break;
		}
		system("cls"); //REMOVE, WINDOWS ONLY!
		printBoard();
		std::cout << "\n";
		int dir;
		std::cout << "Player " << plTurn+1 << "'s turn. " << "Rotate sub board: x:y:dir : ";
		std::cin >> x >> y >> dir;
		rotateSubBoard(x, y, dir);
		hw = hasWonFast();
		if (hw != 0) {
			break;
		}
		plTurn = 1 - plTurn; //Change player
	}
	system("cls");
	printBoard();
	std::cout << "  Player: " << hw << " WON!\n";
}

bool ptg::operator==(const PentagoGame& l1, const PentagoGame& l2)
{
	if (0==memcmp(l1.subBoards, l2.subBoards, sizeof(l1.subBoards))) {
		return true;
	}
	return false;
}
