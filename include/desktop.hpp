#ifndef DESKTOP_HPP
#define DESKTOP_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

struct DesktopIcon {
    sf::RectangleShape body;
    sf::Text label;
    std::string appId;
    int slotIndex;
};

class Desktop {
private:
    sf::RectangleShape background;
    sf::RectangleShape taskbar;
    
    sf::Font font;
    bool isStartMenuOpen;
    sf::RectangleShape startMenuBox;
    sf::RectangleShape settingsItem;
    sf::RectangleShape shutdownItem;
    sf::Text settingsText;
    sf::Text shutdownText;
    sf::RectangleShape startButton;
    sf::Text startText;
    sf::Text clockText;

    std::vector<DesktopIcon> desktopIcons;

    void updateClock();

public:
    Desktop();
    void init(unsigned int width, unsigned int height);
    void createIcon(const std::string& title, const std::string& id);
    
    std::string handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    
    void update();
    void draw(sf::RenderWindow& window);
    void drawStartMenu(sf::RenderWindow& window);
};

#endif