#include "apps/driveRecoveryApp.hpp"
#include <iostream>

DriveRecoveryApp::DriveRecoveryApp() : VirtualWindow("Drive Recovery App", 400, 300) {
    isOpen = false;
    if (!font.loadFromFile("c:/WINDOWS/Fonts/CONSOLA.TTF")) {
        std::cerr << "[ERROR] DriveRecoveryApp FAILED TO LOAD FONT\n";
    }
    
    statusText.setFont(font);
    statusText.setString("Drive Recovery - Not Implemented Yet");
    statusText.setCharacterSize(14);
    statusText.setFillColor(sf::Color::Red);
    statusText.setPosition(20, 40);
}

void DriveRecoveryApp::update() {
    VirtualWindow::update();
}

void DriveRecoveryApp::draw(sf::RenderWindow& window) {
    VirtualWindow::draw(window);
    if (isOpen) {
        window.draw(statusText);
    }
}
