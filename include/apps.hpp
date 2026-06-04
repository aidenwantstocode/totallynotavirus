#ifndef APPS_HPP
#define APPS_HPP

#include "VirtualWindow.hpp"
#include <string>

//notepad app
class NotepadApp : public VirtualWindow {
private:
    sf::Font font;
    sf::Text contentText;

public:
    NotepadApp();
    void update() override;
    void draw(sf::RenderWindow& window) override;
};

//terminal/cmd virus app
class TerminalApp : public VirtualWindow {
private:
    sf::Font font;
    sf::Text terminalText;
    std::string commandHistory;
    std::string currentInput;

public:
    TerminalApp();
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override; // Menangkap ketikan keyboard
    void update() override;
    void draw(sf::RenderWindow& window) override;
};

#endif