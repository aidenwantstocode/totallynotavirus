#ifndef APPS_HPP
#define APPS_HPP

#include "virtualWindow.hpp"
#include <string>

//notepad app
class NotepadApp : public VirtualWindow {
private:
    sf::Font font;
    sf::Text contentText;

public:
    NotepadApp();
    void openFile(const std::string& filename, const std::string& content);
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
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void update() override;
    void draw(sf::RenderWindow& window) override;
};

class DriveRecoveryApp : public VirtualWindow {
private:
    sf::Font font;
    sf::Text statusText;

public:
    DriveRecoveryApp();
    void update() override;
    void draw(sf::RenderWindow& window) override;
};

#endif