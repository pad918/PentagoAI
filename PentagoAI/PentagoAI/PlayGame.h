#pragma once
#include "Minimax.h"
#include "Pentago.h"
#include "MyMath.h"

class GameHandler {
private:

public:
	GameHandler();
	void playAgainstMinimax(ptg::PentagoGame &board, int depth);
};