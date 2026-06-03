#ifndef VIRTUAL_WINDOW_HPP
#define VIRTUAL_WINDOW_HPP

#include <SFML/Graphics.hpp>
#include <string>

class VirtualWindow {
protected:
    sf::RectangleShape windowFrame;
    sf::RectangleShape titleBar;
    sf::Text titleText;
    bool isOpen;
    bool isDragged;

public:
    VirtualWindow(const std::string& title, float width, float height);
    virtual ~VirtualWindow() = default;

    virtual void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    virtual void update();
    virtual void draw(sf::RenderWindow& window);

    bool getIsOpen() const { return isOpen; }
    void setIsOpen(bool open) { isOpen = open; }
};

#endif