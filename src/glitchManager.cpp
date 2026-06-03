#include "GlitchManager.hpp"

GlitchManager::GlitchManager() {
    isGlitching = false;
}

void GlitchManager::triggerJumpscare() {
    isGlitching = true;
    glitchClock.restart();
}

void GlitchManager::update() {
    if (isGlitching && glitchClock.getElapsedTime().asSeconds() > 1.5f) {
        isGlitching = false;
    }
}

void GlitchManager::applyEffect(sf::RenderWindow& window) {
    if (isGlitching) {
    }
}