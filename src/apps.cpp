#include <iostream>
#include "Apps.hpp"

//NOTEPAD APP

NotepadApp::NotepadApp() : VirtualWindow("Notepad - todo_list.txt", 450, 350) {
    windowFrame.setFillColor(sf::Color(255, 255, 255)); 

    if (!font.loadFromFile("c:/WINDOWS/Fonts/CONSOLA.TTF")) {
        std::cerr << "[ERROR] NotepadApp gagal memuat font.\n";
    }

    //notepad text properties
    contentText.setFont(font);
    contentText.setCharacterSize(13);
    contentText.setFillColor(sf::Color(50, 50, 50)); // Warna abu-abu gelap agar nyaman dibaca
    contentText.setLineSpacing(1.2f);

    std::string diaryText = 
        "ADVANCED PROGRAMMING - FINAL PROJECT LOG\n"
        "=========================================\n"
        "Development Status: Stable\n"
        "Current Task:\n"
        " [X] Setup MSYS2 and modern UCRT64 toolchain\n"
        " [X] Build window framework & boundary logic\n"
        " [ ] Implement interactive desktop icons\n"
        " [ ] Read incoming mail from school server\n\n"
        "WARNING:\n"
        "Lately, the system has been generating weird cache files.\n"
        "Do NOT open any suspicious attachments in the email client.\n"
        "The security protocol is highly unstable.";
        
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