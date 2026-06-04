#include "Game.hpp"

Game::Game() {
    initWindow();
    desktop.init(SCREEN_WIDTH, SCREEN_HEIGHT);
    notepad.setHasFocus(true);
    terminal.setHasFocus(false);
}

void Game::initWindow() {
    window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "totallynotavirus", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        //convert mouse position to world coordinates
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

        if (event.type == sf::Event::MouseButtonPressed) {
            std::string clickedApp = desktop.handleInput(pixelPos);
            if (clickedApp == "notepad") {
                notepad.setIsOpen(true);
                notepad.setHasFocus(true);
                terminal.setHasFocus(false);
            }
            if (clickedApp == "cmd") {
                terminal.setIsOpen(true);
                terminal.setHasFocus(true);
                notepad.setHasFocus(false);
            }
        }

        //check if click is on the focused window - if so, block events to other windows
        bool clickOnFocusedWindow = false;
        if (event.type == sf::Event::MouseButtonPressed) {
            if (notepad.getHasFocus() && notepad.getIsOpen() && notepad.containsPoint(mousePos)) {
                clickOnFocusedWindow = true;
            } else if (terminal.getHasFocus() && terminal.getIsOpen() && terminal.containsPoint(mousePos)) {
                clickOnFocusedWindow = true;
            }
        }

        //send events to windows, but prevent propagation through focused window
        if (notepad.getIsOpen()) {
            if (notepad.getHasFocus() || !clickOnFocusedWindow) {
                notepad.handleEvent(event, window);
            }
        }
        if (terminal.getIsOpen()) {
            if (terminal.getHasFocus() || !clickOnFocusedWindow) {
                terminal.handleEvent(event, window);
            }
        }
        
        //ensure focus exclusivity - only one window can have focus
        if (notepad.getHasFocus() && terminal.getHasFocus()) {
            terminal.setHasFocus(false);
        }
    }
}

void Game::update() {
    desktop.update();
    notepad.update();
    terminal.update();
    glitchManager.update();
}

void Game::render() {
    window.clear();
    
    desktop.draw(window);
    
    //draw windows in z-order (unfocused first, focused on top)
    if (notepad.getHasFocus()) {
        terminal.draw(window);
        notepad.draw(window);
    } else {
        notepad.draw(window);
        terminal.draw(window);
    }
    
    glitchManager.applyEffect(window);

    window.display();
}