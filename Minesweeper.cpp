#include <iostream>
#include "Textures.h"
#include "Random.h"
#include "Board.h"
#include "Tile.h"
#include <SFML/Graphics.hpp>

using namespace std;

int main()
{
    Board board;
    sf::RenderWindow window(sf::VideoMode(board.width, board.height), "Minesweeper!");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
                
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i pos = sf::Mouse::getPosition(window);
                if (event.mouseButton.button == sf::Mouse::Left) {
                    board.click(pos, 0);
                }
                else if (event.mouseButton.button == sf::Mouse::Right)  {
                    board.click(pos, 1);
                }
            }

        }

        window.clear();

        board.draw(window);

        window.display();
    }

    Textures::clear();
    return 0;
}