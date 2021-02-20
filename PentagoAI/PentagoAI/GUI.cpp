#include "GUI.h"

GUI::GUI()
{
	marble = sf::RectangleShape(sf::Vector2f(50, 50));
	marble.setFillColor(sf::Color::White);

	if (!motsolsTexture.loadFromFile("sprites/motsols.png")) {
		std::cout << "ERROR: Could not load texture from file\n";
	}
	if (!medsolsTexture.loadFromFile("sprites/medsols.png")) {
		std::cout << "ERROR: Could not load texture from file\n";
	}
	medsols.setTexture(medsolsTexture);
	motsols.setTexture(motsolsTexture);
}

void GUI::update(sf::RenderWindow & window, ptg::PentagoGame & board)
{
	for (int x = 0; x < 6; x++) {
		for (int y = 0; y < 6; y++) {
			marble.setPosition(sf::Vector2f(50 + 50 *x, 50 + 50*y));
			int player = board.marbleAt(x, y);
			marble.setFillColor(player >=2 ? sf::Color::White : 
				player>=1 ? sf::Color::Green : sf::Color::Black);
			window.draw(marble);
		}
	}
	//Draw lines
	for (int x = 1; x <= 7; x++) {
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(50*x, 50)),
			sf::Vertex(sf::Vector2f(50*x, 500))
		};
		window.draw(line, 2, sf::Lines);
	}
	for (int y = 1; y <= 7; y++) {
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(50, 50 * y)),
			sf::Vertex(sf::Vector2f(500, 50 * y))
		};
		window.draw(line, 2, sf::Lines);
	}
	//Subboard separation lines
	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(200, 50), sf::Color::Red),
		sf::Vertex(sf::Vector2f(200, 350),sf::Color::Red)
	};
	window.draw(line, 2, sf::Lines);
	sf::Vertex line1[] =
	{
		sf::Vertex(sf::Vector2f(50, 200), sf::Color::Red),
		sf::Vertex(sf::Vector2f(350, 200), sf::Color::Red)
	};
	window.draw(line1, 2, sf::Lines);

	//Draw rotation buttons
	medsols.setPosition(sf::Vector2f(0, 50));
	window.draw(medsols);
	medsols.setPosition(sf::Vector2f(350, 50));
	window.draw(medsols);
	medsols.setPosition(sf::Vector2f(0, 300));
	window.draw(medsols);
	medsols.setPosition(sf::Vector2f(350, 300));
	window.draw(medsols);
	
	motsols.setPosition(sf::Vector2f(50, 0));
	window.draw(motsols);
	motsols.setPosition(sf::Vector2f(300, 0));
	window.draw(motsols);
	motsols.setPosition(sf::Vector2f(50, 350));
	window.draw(motsols);
	motsols.setPosition(sf::Vector2f(300, 350));
	window.draw(motsols);
	
}

bool GUI::poll(sf::RenderWindow & window, ptg::PentagoGame & board)
{
	//Returns true when player has choosen a move and a rotation.
	if (currentMove == 0) {
		
		auto pos = mouseHandler.getPosition(window);
		pos -= sf::Vector2i(50, 50);
		std::cout << "Clicked at pos = " << pos.x << " " << pos.y << std::endl;
		setMarblePos.x = pos.x / 50;
		setMarblePos.y = pos.y / 50;
		currentMove++;
	}
	else {
		rotateBoard = sf::Vector3i(1, 1, 1);

		auto pos = mouseHandler.getPosition(window);
		//Poll all the buttons OBS uggly code here:

		//Subboard 0: rotate medsols
		if (pos.x > 0 && pos.x < 50 && pos.y > 50 && pos.y < 100) {
			rotateBoard = sf::Vector3i(0, 0, 1);
		}
		//Subboard 1: rotate medsols
		else if (pos.x > 350 && pos.x < 400 && pos.y > 50 && pos.y < 100) {
			rotateBoard = sf::Vector3i(1, 0, 1);
		}
		//Subboard 2: rotate medsols
		else if (pos.x > 0 && pos.x < 50 && pos.y > 300 && pos.y < 350) {
			rotateBoard = sf::Vector3i(0, 1, 1);
		}
		//Subboard 3: rotate medsols
		else if (pos.x > 350 && pos.x < 400 && pos.y > 300 && pos.y < 350) {
			rotateBoard = sf::Vector3i(1, 1, 1);
		}

		/******************************************************************/
		//Subboard 0: rotate motsols
		if (pos.x > 50 && pos.x < 100 && pos.y > 0 && pos.y < 50) {
			rotateBoard = sf::Vector3i(0, 0, -1);
		}
		//Subboard 1: rotate motsols
		else if (pos.x > 300 && pos.x < 350 && pos.y > 0 && pos.y < 50) {
			rotateBoard = sf::Vector3i(1, 0, -1);
		}
		//Subboard 2: rotate motsols
		else if (pos.x > 50 && pos.x < 100 && pos.y > 350 && pos.y < 400) {
			rotateBoard = sf::Vector3i(0, 1, -1);
		}
		//Subboard 3: rotate medsols
		else if (pos.x > 300 && pos.x < 350 && pos.y > 350 && pos.y < 400) {
			rotateBoard = sf::Vector3i(1, 1, -1);
		}


		currentMove = 0;
		return true;
	}
	return false;
}
