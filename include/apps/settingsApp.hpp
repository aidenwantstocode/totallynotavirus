#ifndef SETTINGS_APP_HPP
#define SETTINGS_APP_HPP

#include "virtualWindow.hpp"

class SettingsApp : public VirtualWindow {
private:
    sf::Font font;
    sf::Text diagnosticHeader;
    sf::Text diagnosticInfo;
    
    sf::RectangleShape fullscreenBtn;
    sf::Text fullscreenText;
    
    bool fullscreenRequested;

public:
    SettingsApp();
    void update() override;
    void draw(sf::RenderWindow& window) override;
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;

    bool isFullscreenToggleRequested() const { return fullscreenRequested; }
    void clearFullscreenToggleRequest() { fullscreenRequested = false; }
};

#endif
