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

    int draggedIconIndex = -1;
    sf::Vector2f dragOffset;
    sf::Clock doubleClickTimer;
    int lastClickedIconIndex = -1;

    void updateClock();
    void snapToGrid(DesktopIcon& icon);

public:
    Desktop();
    void init(unsigned int width, unsigned int height);
    
    std::string handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    
    void update();
    void draw(sf::RenderWindow& window);
    
    void createIcon(const std::string& title, const std::string& id, float x, float y);
};

#endif