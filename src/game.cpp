#include <iostream>

#include "game.hpp"

Game::Game() {
    initWindow();
    desktop.init(SCREEN_WIDTH, SCREEN_HEIGHT);
    notepad.setHasFocus(true);
    terminal.setHasFocus(false);
    systemDelayMultiplier = 1.0f;
}

void Game::initWindow() {
    window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "totallynotavirus", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        
        terminal.update();
        notepad.update();
        installerWizard.update();
        
        installerWizard.setSystemCorrupted(isDriveRecoveryCorrupted);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

        if (installerWizard.getIsOpen() && installerWizard.getIsErrorOpen()) {
            installerWizard.handleEvent(event, window);
            continue;
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            std::string clickedApp = desktop.handleInput(pixelPos);
            
            if (clickedApp == "txt_todo") {
                terminal.setHasFocus(false);
                notepad.openFile("todo_list.txt", 
                    "AMITY OS SYSTEM LOG - TODO LIST\n"
                    "===================================\n"
                    "1. Fix terminal installation errors.\n"
                    "2. Run the Drive Recovery Tool.\n"
                    "3. DO NOT look at the basement drive light.");
            }
            
            else if (clickedApp == "txt_log") {
                terminal.setHasFocus(false);
                notepad.openFile("system_log.txt", 
                    "WARNING: Sector 0x04F2 corrupted.\n"
                    "Unidentified logic injection detected in flash device.\n"
                    "Amity Shield Antivirus failed to clean threats (0%).\n"
                    "Anomalous behavior expanding...");
            }
            
            else if (clickedApp == "cmd") {
                terminal.setIsOpen(true);
                terminal.setHasFocus(true);
                notepad.setHasFocus(false);
            }

            else if (clickedApp == "pkg_installer") {
                installerWizard.setIsOpen(true);
                installerWizard.setHasFocus(true);
                
                notepad.setHasFocus(false);
                terminal.setHasFocus(false);
            }
        }

        //check if click is on the focused window - if so, block events to other windows
        bool clickOnFocusedWindow = false;
        if (event.type == sf::Event::MouseButtonPressed) {
            if (notepad.getHasFocus() && notepad.getIsOpen() && notepad.containsPoint(mousePos)) {
                clickOnFocusedWindow = true;
            } else if (terminal.getHasFocus() && terminal.getIsOpen() && terminal.containsPoint(mousePos)) {
                clickOnFocusedWindow = true;
            } else if (installerWizard.getHasFocus() && installerWizard.getIsOpen() && installerWizard.containsPoint(mousePos)) {
                clickOnFocusedWindow = true;
            }
        }

        //send events to windows, but prevent propagation through focused window
        if (installerWizard.getIsOpen() && installerWizard.getIsErrorOpen()) {
            installerWizard.handleEvent(event, window);
            continue;
        }

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
        if (installerWizard.getIsOpen()) {
            if (installerWizard.getHasFocus() || !clickOnFocusedWindow) {
                installerWizard.handleEvent(event, window);
                //safeguard (recalculate performance)
                if (installerWizard.getIsFinalized() && systemDelayMultiplier == 1.0f) {
                    recalculateSystemPerformance();
                }
            }
        }
        
        //ensure focus exclusivity - only one window can have focus
        if (notepad.getHasFocus() && terminal.getHasFocus()) {
            terminal.setHasFocus(false);
        }
        if (notepad.getHasFocus() && installerWizard.getHasFocus()) {
            installerWizard.setHasFocus(false);
        }
        if (terminal.getHasFocus() && installerWizard.getHasFocus()) {
            installerWizard.setHasFocus(false);
        }
    }
}

void Game::update() {
    desktop.update();
    notepad.update();
    terminal.update();
    installerWizard.update();
    glitchManager.update();
}

void Game::render() {
    window.clear();
    
    desktop.draw(window);
    
    //draw windows in z-order (unfocused first, focused on top)
    if (installerWizard.getHasFocus() && installerWizard.getIsOpen()) {
        notepad.draw(window);
        terminal.draw(window);
        installerWizard.draw(window);
    } else if (notepad.getHasFocus()) {
        terminal.draw(window);
        installerWizard.draw(window);
        notepad.draw(window);
    } else {
        notepad.draw(window);
        installerWizard.draw(window);
        terminal.draw(window);
    }
    
    glitchManager.applyEffect(window);

    window.display();
}

void Game::recalculateSystemPerformance() {
    float newMultiplier = 1.0f; //1.0f = 100% normal speed

    if (installerWizard.isComponentChecked("antivirus")) {
        newMultiplier += 0.5f; //system instability +50%
        std::cout << "[OS Kernel] Amity Shield Antivirus detected. Allocating memory...\n";
    }
    if (installerWizard.isComponentChecked("health_monitor")) {
        newMultiplier += 0.3f; // system instability +30%
        std::cout << "[OS Kernel] Marrow Health Monitor active. Scanning cycles...\n";
    }
    if (installerWizard.isComponentChecked("abstractor")) {
        newMultiplier += 0.4f; //system instability +40%
        std::cout << "[OS Kernel] Memory Abstractor engaged. Shifting heap...\n";
    }

    systemDelayMultiplier = newMultiplier;
    terminal.setDelayMultiplier(systemDelayMultiplier);
    std::cout << "[OS Kernel] TOTAL SYSTEM DELAY MULTIPLIER: " << systemDelayMultiplier << "x\n";
}