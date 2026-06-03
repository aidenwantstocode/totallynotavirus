#ifndef DESKTOP_HPP
#define DESKTOP_HPP

#include <SFML/Graphics.hpp>

class Desktop {
private:
    sf::RectangleShape background;
    sf::RectangleShape taskbar;
    sf::Font font;
    sf::RectangleShape startButton;
    sf::Text startText;
    sf::Text clockText;

    void updateClock();

public:
    Desktop();
    void init(unsigned int width, unsigned int height);
    void handleInput(sf::Vector2i mousePos);
    void update();
    void draw(sf::RenderWindow& window);
};

#endif