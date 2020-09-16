#include "Pentago.h"

ptg::PentagoSubBoard::PentagoSubBoard()
{
	//Set all marbles poitions to zero, meaning no marble
	memset(marbles, 0, sizeof(unsigned char)*9);
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

int ptg::PentagoGame::maxMarblesInARow(int player, int total, int dir, int x, int y)
{
	//Recursive function
	int max = 0;
	if (dir < 0) {
		int dir1 = maxMarblesInARow(player, 0, 0, x, y);
		/*int dir2 = maxMarblesInARow(player, 0, 1, x, y);
		int dir3 = maxMarblesInARow(player, 0, 2, x, y);
		int dir4 = maxMarblesInARow(player, 0, 3, x, y);
		*/
		max = dir1;
		/*if (dir2 > max) { max = dir2; }
		if (dir3 > max) { max = dir3; }
		if (dir4 > max) { max = dir4; }
		*/
	}
	else {
		int subBoardPos = 0;
		int subBoardXPos = 0;
		int subBoardYPos = 0;
		switch (dir)
		{
		case 0:
			x++;
			subBoardPos =  (y / 3) * 2 + (x / 3) * 1;
			subBoardXPos = (x % 3);
			subBoardYPos = (y % 3);
			if (subBoards[subBoardPos].marbles[subBoardYPos * 3 + subBoardXPos] == player) {
				total = maxMarblesInARow(player, total+1, dir, x, y);
				
			}
			else {
				return total;
			}
			break;
		default:
			return total;
			break;
		}
	}
	if (total > max) { max = total; }
	return max;
}

int ptg::PentagoGame::hasWon()
{
	//Test player one:
	//TEST...

	return maxMarblesInARow(1, 0, -1, 0, 0);
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

void ptg::PentagoGame::playManualGame()
{
	setMarble(0, 0, 1);
	setMarble(1, 0, 1);
	setMarble(2, 0, 1);

	int plTurn = 0;
	for (int i = 0; i < 36; i++) {
		system("cls"); //REMOVE SHITTY WINDOWS ONLY SHIT!
		//TEST
		std::cout << "max in a row: " << hasWon() << "\n";
		//END OF TEST
		printBoard();
		std::cout << "\n";
		int x, y, val;
		std::cout << "Player " << plTurn  << "'s turn. " << "Set marble: x:y:val : ";
		std::cin >> x >> y >> val;
		setMarble(x, y, val);
		system("cls"); //REMOVE SHITTY WINDOWS ONLY SHIT!
		printBoard();
		std::cout << "\n";
		int dir;
		std::cout << "Player " << plTurn << "'s turn. " << "Rotate sub board: x:y:dir : ";
		std::cin >> x >> y >> dir;
		rotateSubBoard(x, y, dir);
		plTurn = 1 - plTurn; //Change player
	}
}


