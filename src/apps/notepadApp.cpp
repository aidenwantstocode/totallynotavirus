#include "apps/notepadApp.hpp"
#include <iostream>

NotepadApp::NotepadApp() : VirtualWindow("Notepad", 400, 300) {
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

void NotepadApp::openFile(const std::string& filename, const std::string& content) {
    titleText.setString(filename + " - Notepad");
    
    // Simple line wrapping
    std::string wrappedContent = "";
    float maxWidth = windowFrame.getSize().x - 20;
    
    sf::Text tempText = contentText;
    size_t pos = 0;
    while (pos < content.length()) {
        size_t nextSpace = content.find(' ', pos);
        if (nextSpace == std::string::npos) nextSpace = content.length();
        
        std::string word = content.substr(pos, nextSpace - pos);
        tempText.setString(wrappedContent + word);
        
        if (tempText.getLocalBounds().width > maxWidth) {
            wrappedContent += "\n" + word + " ";
        } else {
            wrappedContent += word + " ";
        }
        
        pos = nextSpace + 1;
    }
    
    contentText.setString(wrappedContent);
}

void NotepadApp::update() {
    VirtualWindow::update();
    if (isOpen) {
        contentText.setPosition(windowFrame.getPosition().x + 10, windowFrame.getPosition().y + 35);
    }
}

void NotepadApp::draw(sf::RenderWindow& window) {
    VirtualWindow::draw(window);
    if (isOpen) {
        window.draw(contentText);
    }
}
