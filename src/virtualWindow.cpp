#include "VirtualWindow.hpp"

VirtualWindow::VirtualWindow(const std::string& title, float width, float height) {
    isOpen = true;
    isDragged = false;

    windowFrame.setSize(sf::Vector2f(width, height));
    windowFrame.setFillColor(sf::Color::White);
    windowFrame.setOutlineThickness(2);
    windowFrame.setOutlineColor(sf::Color::Black);

    titleBar.setSize(sf::Vector2f(width, 30));
    titleBar.setFillColor(sf::Color::Blue);
}

void VirtualWindow::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
}

void VirtualWindow::update() {
}

void VirtualWindow::draw(sf::RenderWindow& window) {
    if (!isOpen) return;
    window.draw(windowFrame);
    window.draw(titleBar);
}