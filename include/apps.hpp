#ifndef APPS_HPP
#define APPS_HPP

#include "VirtualWindow.hpp"

//notepad app
class NotepadApp : public VirtualWindow {
private:
    sf::Text contentText;
public:
    NotepadApp();
    void update() override; // Polimorfisme: Mengganti fungsi update bawaan induk
    void draw(sf::RenderWindow& window) override;
};

//terminal/cmd virus app
class TerminalApp : public VirtualWindow {
private:
    sf::Text commandLineText;
public:
    TerminalApp();
    void update() override;
    void draw(sf::RenderWindow& window) override;
};

#endif