#ifndef VIRTUAL_WINDOW_HPP
#define VIRTUAL_WINDOW_HPP

#include <SFML/Graphics.hpp>
#include <string>

class VirtualWindow {
protected:
    sf::RectangleShape windowFrame;
    sf::RectangleShape titleBar;
    sf::RectangleShape closeButton;
    sf::Font font;
    sf::Text titleText;
    sf::Text closeText;

    bool isOpen;
    bool isDragged;
    bool hasFocus;
    sf::Vector2f dragOffset;

public:
    VirtualWindow(const std::string& title, float width, float height);
    virtual ~VirtualWindow() = default;

    //click and drag mouse FOR THIS WINDOW
    virtual void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    virtual void update();
    virtual void draw(sf::RenderWindow& window);

    bool getIsOpen() const { return isOpen; }
    void setIsOpen(bool open) { isOpen = open; }
    bool getHasFocus() const { return hasFocus; }
    void setHasFocus(bool focus) { hasFocus = focus; }
    void setPosition(float x, float y);
    bool containsPoint(sf::Vector2f point) const { return windowFrame.getGlobalBounds().contains(point); }
};

#endif