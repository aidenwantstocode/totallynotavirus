#include <iostream>
#include "Apps.hpp"

//NOTEPAD APP

NotepadApp::NotepadApp() : VirtualWindow("Notepad", 450, 350) {
    isOpen = false;
    windowFrame.setFillColor(sf::Color(255, 255, 255)); 

    if (!font.loadFromFile("c:/WINDOWS/Fonts/CONSOLA.TTF")) {
        std::cerr << "[ERROR] NotepadApp FAILED TO LOAD FONT\n";
    }

    contentText.setFont(font);
    contentText.setCharacterSize(12);
    contentText.setFillColor(sf::Color(50, 50, 50));
    contentText.setLineSpacing(1.2f);
    contentText.setString("");
}

//Dynamic function for opening files in notepad with different content based on filename
void NotepadApp::openFile(const std::string& filename, const std::string& content) {
    titleText.setString("Notepad - " + filename);
    contentText.setString(content);
    isOpen = true;
    hasFocus = true;
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

TerminalApp::TerminalApp() : VirtualWindow("Terminal", 520, 380) {
    isOpen = false; 
    windowFrame.setFillColor(sf::Color::Black);
    titleBar.setFillColor(sf::Color(45, 45, 45));

    if (!font.loadFromFile("c:/WINDOWS/Fonts/CONSOLA.TTF")) {
        std::cerr << "[ERROR] TerminalApp failed to load font.\n";
    }

    terminalText.setFont(font);
    terminalText.setCharacterSize(12);
    terminalText.setFillColor(sf::Color(255, 255, 255));
    terminalText.setLineSpacing(1.1f);

    commandHistory = "Amity Operating System [build 13a94]\n(c) Marrow Computers 1994. All rights reserved.\n\nC:\\> ";
    currentInput = "";
    terminalText.setString(commandHistory + currentInput);
}

void TerminalApp::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    VirtualWindow::handleEvent(event, window);

    if (!isOpen) return;

    if (event.type == sf::Event::TextEntered) {
        
        if (event.text.unicode == 8) {
            if (!currentInput.empty()) {
                currentInput.pop_back();
            }
        }
        else if (event.text.unicode == 13 || event.text.unicode == 10) {
            if (currentInput == "help") {
                commandHistory += currentInput + "\nAvailable commands:\n  help   - Show documentation\n  status - Check virtual OS integrity\n  clear  - Clear screen buffer\n\nC:\\> ";
            } 
            else if (currentInput == "clear") {
                commandHistory = "Amity Operating System [build 13a94]\n(c) Marrow Computers 1994. All rights reserved.\n\nC:\\> ";
            } 
            else if (currentInput == "status") {
                commandHistory += currentInput + "\n[SYSTEM STATUS]: INTEGRITY SECURE\n[FIREWALL]: SHIELD ACTIVE (100%)\n\nC:\\> ";
            } 
            else if (!currentInput.empty()) {
                commandHistory += currentInput + "\n'" + currentInput + "' is not recognized as an internal or external command.\n\nC:\\> ";
            } 
            else {
                commandHistory += "\nC:\\> ";
            }
            
            currentInput = "";
        }
        else if (event.text.unicode < 128 && event.text.unicode >= 32) {
            currentInput += static_cast<char>(event.text.unicode);
        }

        terminalText.setString(commandHistory + currentInput);
    }
}

void TerminalApp::update() {
    VirtualWindow::update();
    if (isOpen) {
        terminalText.setPosition(windowFrame.getPosition().x + 12, windowFrame.getPosition().y + 40);
    }
}

void TerminalApp::draw(sf::RenderWindow& window) {
    VirtualWindow::draw(window);
    if (isOpen) {
        window.draw(terminalText);
    }
}