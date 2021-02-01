#include "GUI.h"

GUI::GUI()
{
	marble = sf::RectangleShape(sf::Vector2f(50, 50));
	marble.setFillColor(sf::Color::White);
}

void GUI::update(sf::RenderWindow & window, ptg::PentagoGame & board)
{
	for (int x = 0; x < 6; x++) {
		for (int y = 0; y < 6; y++) {
			marble.setPosition(sf::Vector2f(50 *x, 50*y));
			int player = board.marbleAt(x, y);
			marble.setFillColor(player >=2 ? sf::Color::White : 
				player>=1 ? sf::Color::Green : sf::Color::Black);
			window.draw(marble);
		}
	}
}
