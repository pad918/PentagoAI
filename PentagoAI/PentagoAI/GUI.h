#pragma once
#include <SFML/Graphics.hpp>
#include "Pentago.h"

class GUI {
private:
	sf::RectangleShape marble;
	int currentMove = 0;
	sf::Mouse mouseHandler;
	sf::Texture medsolsTexture, motsolsTexture;
	sf::Sprite medsols, motsols;
public:
	GUI();
	void update(sf::RenderWindow & window, ptg::PentagoGame & board);
	bool poll(sf::RenderWindow & window, ptg::PentagoGame & board);
	bool wasMoveLegal;
	sf::Vector2i setMarblePos;
	sf::Vector3i rotateBoard;
};