#include "apps/settingsApp.hpp"
#include <iostream>

SettingsApp::SettingsApp() : VirtualWindow("Control Panel", 320, 260) {
    isOpen = false;
    fullscreenRequested = false;
    windowFrame.setFillColor(sf::Color(192, 192, 192));

    if (!font.loadFromFile("c:/WINDOWS/Fonts/CONSOLA.TTF")) {
        std::cerr << "[ERROR] SettingsApp FAILED TO LOAD FONT\n";
    }

    diagnosticHeader.setFont(font);
    diagnosticHeader.setString("SYSTEM DIAGNOSTICS:");
    diagnosticHeader.setCharacterSize(13);
    diagnosticHeader.setFillColor(sf::Color(0, 0, 128));
    diagnosticHeader.setStyle(sf::Text::Bold);

    diagnosticInfo.setFont(font);
    diagnosticInfo.setString(
        "OS Version: Amity OS v1.02\n"
        "Processor: Amity DX4 @ 100MHz\n"
        "Base Memory: 640KB RAM OK\n"
        "Resolution: 1024x768 (Fixed)\n"
        "Drivers: Recovery v0.1a"
    );
    diagnosticInfo.setCharacterSize(11);
    diagnosticInfo.setFillColor(sf::Color::Black);

    fullscreenBtn.setSize(sf::Vector2f(170.f, 30.f));
    fullscreenBtn.setFillColor(sf::Color(220, 220, 220));
    fullscreenBtn.setOutlineThickness(1.5f);
    fullscreenBtn.setOutlineColor(sf::Color::Black);

    fullscreenText.setFont(font);
    fullscreenText.setString("Toggle Fullscreen");
    fullscreenText.setCharacterSize(12);
    fullscreenText.setFillColor(sf::Color::Black);
}

void SettingsApp::update() {
    // No continuous logic needed for now
}

void SettingsApp::draw(sf::RenderWindow& window) {
    if (!isOpen) return;
    VirtualWindow::draw(window);

    sf::Vector2f winPos = windowFrame.getPosition();

    diagnosticHeader.setPosition(winPos.x + 20.f, winPos.y + 40.f);
    diagnosticInfo.setPosition(winPos.x + 20.f, winPos.y + 65.f);
    fullscreenBtn.setPosition(winPos.x + 20.f, winPos.y + 190.f);
    fullscreenText.setPosition(winPos.x + 35.f, winPos.y + 197.f);

    window.draw(diagnosticHeader);
    window.draw(diagnosticInfo);
    window.draw(fullscreenBtn);
    window.draw(fullscreenText);
}

void SettingsApp::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    VirtualWindow::handleEvent(event, window);

    if (!isOpen) return;

    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF = window.mapPixelToCoords(pixelPos);

    sf::Vector2f btnPos = windowFrame.getPosition() + sf::Vector2f(20.f, 190.f);
    sf::FloatRect btnBounds(btnPos.x, btnPos.y, fullscreenBtn.getSize().x, fullscreenBtn.getSize().y);

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (btnBounds.contains(mousePosF)) {
            std::cout << "[Control Panel] Fullscreen toggled requested.\n";
            fullscreenRequested = true;
            fullscreenBtn.setFillColor(sf::Color(150, 150, 150));
        }
    } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        fullscreenBtn.setFillColor(sf::Color(220, 220, 220));
    } else if (event.type == sf::Event::MouseMoved) {
        if (btnBounds.contains(mousePosF)) {
            fullscreenBtn.setFillColor(sf::Color(200, 200, 200));
        } else {
            fullscreenBtn.setFillColor(sf::Color(220, 220, 220));
        }
    }
}
