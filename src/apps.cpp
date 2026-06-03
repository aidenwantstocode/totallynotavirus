#include "Apps.hpp"

NotepadApp::NotepadApp() : VirtualWindow("Notepad - secret.txt", 400, 300) {
    windowFrame.setFillColor(sf::Color(255, 255, 240)); 
}

void NotepadApp::update() {
    VirtualWindow::update(); 
}

void NotepadApp::draw(sf::RenderWindow& window) {
    VirtualWindow::draw(window); 

}

TerminalApp::TerminalApp() : VirtualWindow("Command Prompt", 500, 350) {
    windowFrame.setFillColor(sf::Color::Black); 
    titleBar.setFillColor(sf::Color(64, 64, 64)); 
}

void TerminalApp::update() {
    VirtualWindow::update();
}

void TerminalApp::draw(sf::RenderWindow& window) {
    VirtualWindow::draw(window);
}