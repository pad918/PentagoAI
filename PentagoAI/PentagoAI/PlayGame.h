#pragma once
#include <Eigen/Dense>
#include "Minimax.h"
#include "Pentago.h"
#include "MyMath.h"
#include "NeuralNetwork.h"

class Player {
protected:

public:
	virtual void doMove(ptg::PentagoGame & board) = 0;
};

class HumanPlayer : public Player {
private:

public:
	void doMove(ptg::PentagoGame & board);
};

class NNPlayer : public Player {
private:
	NeuralNetwork * nn;
public:
	NNPlayer();
	void doMove(ptg::PentagoGame & board);

};

class MinimaxPlayer : public Player {
private:
	mm::Minimax ai;
	int maxDepth = 4;
public:
	void doMove(ptg::PentagoGame & board);
};

class GameHandler {
private:
	Player * playerOne, * playerTwo;
public:
	GameHandler();
	void playGame(int playerOneType, int playerTwoType);
	void playAgainstMinimax(ptg::PentagoGame &board, int depth);
};