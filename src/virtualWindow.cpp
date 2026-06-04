#include "VirtualWindow.hpp"
#include <iostream>

VirtualWindow::VirtualWindow(const std::string& title, float width, float height) {
    isOpen = true;
    isDragged = false;

    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "[ERROR] VirtualWindow gagal memuat font.\n";
    }

    // outside window frame
    windowFrame.setSize(sf::Vector2f(width, height));
    windowFrame.setFillColor(sf::Color(240, 240, 240));
    windowFrame.setOutlineThickness(2);
    windowFrame.setOutlineColor(sf::Color::Black);

    //upper bar title
    titleBar.setSize(sf::Vector2f(width, 30));
    titleBar.setFillColor(sf::Color(0, 0, 128));

    //window title text
    titleText.setFont(font);
    titleText.setString(title);
    titleText.setCharacterSize(14);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);

    //'X' button
    closeButton.setSize(sf::Vector2f(22, 22));
    closeButton.setFillColor(sf::Color(192, 192, 192));
    closeButton.setOutlineThickness(1);
    closeButton.setOutlineColor(sf::Color::Black);

    closeText.setFont(font);
    closeText.setString("X");
    closeText.setCharacterSize(12);
    closeText.setFillColor(sf::Color::Black);
    closeText.setStyle(sf::Text::Bold);

    //window default position
    setPosition(200, 150);
}

void VirtualWindow::setPosition(float x, float y) {
    windowFrame.setPosition(x, y);
    titleBar.setPosition(x, y);
    titleText.setPosition(x + 10, y + 5);
    closeButton.setPosition(x + windowFrame.getSize().x - 26, y + 4);
    closeText.setPosition(x + windowFrame.getSize().x - 20, y + 7);
}

void VirtualWindow::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (!isOpen) return;

    //convert mouse position to world coordinates
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

    //mouse click logic
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        
        //close window check
        if (closeButton.getGlobalBounds().contains(mousePos)) {
            isOpen = false;
            std::cout << "[OS Engine] Jendela ditutup oleh pemain.\n";
            return;
        }

        //title bar drag check
        if (titleBar.getGlobalBounds().contains(mousePos)) {
            isDragged = true;
            dragOffset = mousePos - windowFrame.getPosition();
        }
    }

    //drag release logic
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        isDragged = false;
    }

    //drag movement logic
    if (isDragged && event.type == sf::Event::MouseMoved) {
        float newX = mousePos.x - dragOffset.x;
        float newY = mousePos.y - dragOffset.y;
        
        //lock window inside the main render window bounds
        if (newX < 0) newX = 0;
        if (newY < 0) newY = 0;

        if (newX + windowFrame.getSize().x > window.getSize().x)
            newX = window.getSize().x - windowFrame.getSize().x;

        if (newY + windowFrame.getSize().y > window.getSize().y - 40)
            newY = (window.getSize().y - 40) - windowFrame.getSize().y;

        setPosition(newX, newY);
    }
}

void VirtualWindow::update() {
    //upcoming: window animation effects
}

void VirtualWindow::draw(sf::RenderWindow& window) {
    if (!isOpen) return;

    window.draw(windowFrame);
    window.draw(titleBar);
    window.draw(titleText);
    window.draw(closeButton);
    window.draw(closeText);
}