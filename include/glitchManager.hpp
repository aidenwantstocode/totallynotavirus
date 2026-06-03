#ifndef GLITCH_MANAGER_HPP
#define GLITCH_MANAGER_HPP

#include <SFML/Graphics.hpp>

class GlitchManager {
private:
    sf::Clock glitchClock;
    bool isGlitching;

public:
    GlitchManager();
    void triggerJumpscare();
    void update();
    void applyEffect(sf::RenderWindow& window);
};

#endif