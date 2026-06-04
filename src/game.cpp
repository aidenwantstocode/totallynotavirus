#include "Game.hpp"

Game::Game() {
    initWindow();
    desktop.init(SCREEN_WIDTH, SCREEN_HEIGHT);
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
        notepad.handleEvent(event, window);

        if (event.type == sf::Event::MouseButtonPressed) {
            std::string clickedApp = desktop.handleInput(sf::Mouse::getPosition(window));
            if (clickedApp == "notepad") {
                notepad.setIsOpen(true);
            }
        }
    }
}

void Game::update() {
    desktop.update();
    notepad.update();
    glitchManager.update();
}

void Game::render() {
    window.clear();
    
    desktop.draw(window);
    notepad.draw(window);
    glitchManager.applyEffect(window);

    window.display();
}