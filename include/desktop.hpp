#ifndef DESKTOP_HPP
#define DESKTOP_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

struct DesktopIcon {
    sf::RectangleShape body;
    sf::Text label;
    std::string appId;
};

class Desktop {
private:
    sf::RectangleShape background;
    sf::RectangleShape taskbar;
    
    sf::Font font;
    sf::RectangleShape startButton;
    sf::Text startText;
    sf::Text clockText;

    std::vector<DesktopIcon> desktopIcons;

    void updateClock();

public:
    Desktop();
    void init(unsigned int width, unsigned int height);
    
    std::string handleInput(sf::Vector2i mousePos);
    
    void update();
    void draw(sf::RenderWindow& window);
    
    void createIcon(const std::string& title, const std::string& id, float x, float y);
};

#endif