#pragma once
#include <Eigen/Dense>
#include <SFML/Graphics.hpp>
#include "Minimax.h"
#include "Pentago.h"
#include "MyMath.h"
#include "NeuralNetwork.h"
#include "GUI.h"

class Player {
protected:

public:
	int maxDepth = 1;
	int playerVal = 0;
	virtual void doMove(ptg::PentagoGame & board) = 0;
	bool isPollingInput = false;
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
public:
	void doMove(ptg::PentagoGame & board);

};

class GameHandler {
private:
	Player * playerOne, * playerTwo;
	Player * players[2];
	sf::RenderWindow * window;
	bool gameOver = false;
public:
	GameHandler();
	void playGame(int playerOneType, int playerTwoType);
};