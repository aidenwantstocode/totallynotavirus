#include "virtualWindow.hpp"
#include <iostream>
#include <cmath>

float VirtualWindow::ramLagMultiplier = 1.0f;

VirtualWindow::VirtualWindow(const std::string& title, float width, float height) {
    isOpen = true;
    isDragged = false;    hasFocus = false;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "[ERROR] VirtualWindow FAILED TO LOAD FONT\n";
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
    currentPos = sf::Vector2f(200.f, 150.f);
    targetPos = currentPos;
    windowFrame.setPosition(currentPos);
    titleBar.setPosition(currentPos);
    titleText.setPosition(currentPos.x + 10.f, currentPos.y + 5.f);
    closeButton.setPosition(currentPos.x + width - 26.f, currentPos.y + 4.f);
    closeText.setPosition(currentPos.x + width - 20.f, currentPos.y + 7.f);
}

void VirtualWindow::triggerOpenAnimation(sf::Vector2f iconPos) {
    isOpening = true;
    animTime = 0.f;
    animStartPos = iconPos;

    float delay = (ramLagMultiplier - 2.5f) * 0.22f;
    if (delay < 0.f) delay = 0.f;
    if (delay > 2.5f) delay = 2.5f;
    openDelayTimer = delay;

    if (openDelayTimer > 0.f) {
        std::cout << "[OS Kernel] Thinking delay: " << openDelayTimer << "s under conventional load (multiplier: " << ramLagMultiplier << "x)\n";
    }
}

void VirtualWindow::setPosition(float x, float y) {
    targetPos = sf::Vector2f(x, y);
    currentPos = targetPos;
    
    windowFrame.setPosition(currentPos);
    titleBar.setPosition(currentPos);
    titleText.setPosition(currentPos.x + 10.f, currentPos.y + 5.f);
    closeButton.setPosition(currentPos.x + windowFrame.getSize().x - 26.f, currentPos.y + 4.f);
    closeText.setPosition(currentPos.x + windowFrame.getSize().x - 20.f, currentPos.y + 7.f);
}

void VirtualWindow::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (!isOpen) return;

    //convert mouse position to world coordinates
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

    sf::Vector2f winPos = windowFrame.getPosition();
    float winWidth = windowFrame.getSize().x;
    float winHeight = windowFrame.getSize().y;

    //check title bar click for drag
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (windowFrame.getGlobalBounds().contains(mousePos)) {
            hasFocus = true;
        } else {
            hasFocus = false;
        }

        if (closeButton.getGlobalBounds().contains(mousePos)) {
            isOpen = false;
            std::cout << "[OS Engine] VirtualWindow closed by user.\n";
            return;
        }

        sf::FloatRect titleBarBounds(winPos.x, winPos.y, winWidth - 30.f, 30.f);
        if (hasFocus && titleBarBounds.contains(mousePos)) {
            isDragged = true;
            dragOffset = mousePos - winPos;
        }

        // Snap window confinement constraint on release of mouse drag
    } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        if (isDragged) {
            isDragged = false;
            
            float x = winPos.x;
            float y = winPos.y;

            float minY = 0.f;
            float maxY = 768.f - 40.f - 30.f;
            if (isModalActive()) {
                maxY = 768.f - 40.f - winHeight;
            }

            if (y < minY) y = minY;
            if (y > maxY) y = maxY;

            float minX = -winWidth + 40.f;
            float maxX = 1024.f - 40.f;
            if (isModalActive()) {
                minX = 0.f;
                maxX = 1024.f - winWidth;
            }

            if (x < minX) x = minX;
            if (x > maxX) x = maxX;

            setPosition(x, y);
        }
    }

    //drag movement logic
    if (isDragged && event.type == sf::Event::MouseMoved) {
        float newX = mousePos.x - dragOffset.x;
        float newY = mousePos.y - dragOffset.y;
        setPosition(newX, newY);
    }
}

void VirtualWindow::update() {
    if (isOpening) {
        static sf::Clock animClock;
        float dt = animClock.restart().asSeconds();
        if (dt > 0.1f) dt = 0.1f;

        if (openDelayTimer > 0.f) {
            openDelayTimer -= dt;
            if (openDelayTimer < 0.f) openDelayTimer = 0.f;
            return;
        }

        animTime += dt * 12.0f;
        if (animTime >= 1.0f) {
            animTime = 1.0f;
            isOpening = false;
        }
    }
}

void VirtualWindow::draw(sf::RenderWindow& window) {
    if (!isOpen) return;

    if (isOpening) {
        if (openDelayTimer > 0.f) {
            return;
        }

        sf::FloatRect finalBounds = windowFrame.getGlobalBounds();
        float x = animStartPos.x + (finalBounds.left - animStartPos.x) * animTime;
        float y = animStartPos.y + (finalBounds.top - animStartPos.y) * animTime;
        float w = 40.f + (finalBounds.width - 40.f) * animTime;
        float h = 40.f + (finalBounds.height - 40.f) * animTime;

        sf::RectangleShape wireframe(sf::Vector2f(w, h));
        wireframe.setPosition(x, y);
        wireframe.setFillColor(sf::Color::Transparent);
        wireframe.setOutlineThickness(1.5f);
        wireframe.setOutlineColor(sf::Color(0, 0, 128));
        window.draw(wireframe);
        return;
    }

    window.draw(windowFrame);
    window.draw(titleBar);
    window.draw(titleText);
    window.draw(closeButton);
    window.draw(closeText);
}