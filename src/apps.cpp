#include <iostream>
#include "Apps.hpp"

//NOTEPAD APP

NotepadApp::NotepadApp() : VirtualWindow("Notepad - todo_list.txt", 450, 350) {
    windowFrame.setFillColor(sf::Color(255, 255, 255)); 

    if (!font.loadFromFile("c:/WINDOWS/Fonts/CONSOLA.TTF")) {
        std::cerr << "[ERROR] NotepadApp FAILED TO LOAD FONT\n";
    }

    //notepad text properties
    contentText.setFont(font);
    contentText.setCharacterSize(13);
    contentText.setFillColor(sf::Color(50, 50, 50)); // Warna abu-abu gelap agar nyaman dibaca
    contentText.setLineSpacing(1.2f);

    std::string diaryText = 
        "lorem ipsum dolor sit amet, consectetur adipiscing elit. ";
        
    contentText.setString(diaryText);
}

void NotepadApp::update() {
    VirtualWindow::update(); 
    if (isOpen) {
        contentText.setPosition(windowFrame.getPosition().x + 15, windowFrame.getPosition().y + 45);
    }
}

void NotepadApp::draw(sf::RenderWindow& window) {
    VirtualWindow::draw(window); 

    if (isOpen) {
        window.draw(contentText);
    }
}

//TERMINAL APP

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