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
    sf::Text metricsText;
    bool showMetrics;
    float metricsCpu;
    float metricsRam;
    float metricsTemp;

    std::vector<DesktopIcon> desktopIcons;

    void updateClock();

public:
    Desktop();
    void init(unsigned int width, unsigned int height);
    void createIcon(const std::string& title, const std::string& id);
    void removeIcon(const std::string& id);
    sf::Vector2f getIconPosition(const std::string& appId) const;
    
    std::string handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    
    void update();
    void draw(sf::RenderWindow& window);
    void drawStartMenu(sf::RenderWindow& window);
    void setCorruptedTheme(bool enabled);
    void setMetrics(float cpu, float ram, float temp, bool show);
};

#endif