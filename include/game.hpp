#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "desktop.hpp"
#include "glitchManager.hpp"
#include "coreApps.hpp"

class Game {
private:
    sf::RenderWindow window;
    Desktop desktop;
    GlitchManager glitchManager;
    NotepadApp notepad;
    TerminalApp terminal;
    
    const unsigned int SCREEN_WIDTH = 1024;
    const unsigned int SCREEN_HEIGHT = 768;

    void initWindow();

public:
    Game();
    void run();

private:
    void processEvents();
    void update();
    void render();
};

#endif