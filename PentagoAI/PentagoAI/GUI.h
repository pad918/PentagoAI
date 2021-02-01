#pragma once
#include <SFML/Graphics.hpp>
#include "Pentago.h"

class GUI {
private:
	sf::RectangleShape marble;
public:
	GUI();
	void update(sf::RenderWindow & window, ptg::PentagoGame & board);
	void poll(sf::Event & event, ptg::PentagoGame & board);

};