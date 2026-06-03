#include "Desktop.hpp"

Desktop::Desktop() {}

void Desktop::init(unsigned int width, unsigned int height) {
    //os background (teal)
    background.setSize(sf::Vector2f(width, height));
    background.setFillColor(sf::Color(0, 128, 128));

    //lower taskbar (gray)
    taskbar.setSize(sf::Vector2f(width, 40));
    taskbar.setPosition(0, height - 40);
    taskbar.setFillColor(sf::Color(192, 192, 192));
}

void Desktop::handleInput(sf::Vector2i mousePos) {
    //upcoming: icon click logic
}

void Desktop::update() {
    //upcoming: clock/taskbar updates logic
}

void Desktop::draw(sf::RenderWindow& window) {
    window.draw(background);
    window.draw(taskbar);
}