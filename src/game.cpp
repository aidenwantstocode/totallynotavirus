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
        update();
        
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

            else if (clickedApp == "file_explorer") {
                fileExplorer.setIsOpen(true);
                fileExplorer.setHasFocus(true);

                notepad.setHasFocus(false);
                terminal.setHasFocus(false);
                installerWizard.setHasFocus(false);
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
            } else if (fileExplorer.getHasFocus() && fileExplorer.getIsOpen() && fileExplorer.containsPoint(mousePos)) {
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
                // Safeguard: recalculate once after finalization.
                if (installerWizard.getIsFinalized() && !hasRecalculatedPerformance) {
                    recalculateSystemPerformance();
                    hasRecalculatedPerformance = true;
                }
            }
        }
        if (fileExplorer.getIsOpen()) {
            if (fileExplorer.getHasFocus() || !clickOnFocusedWindow) {
                fileExplorer.handleEvent(event, window);
            }
        }
        
        //ensure focus exclusivity - only one window can have focus
        if (notepad.getHasFocus() && terminal.getHasFocus()) {
            terminal.setHasFocus(false);
        }
        if (notepad.getHasFocus() && installerWizard.getHasFocus()) {
            installerWizard.setHasFocus(false);
        }
        if (notepad.getHasFocus() && fileExplorer.getHasFocus()) {
            fileExplorer.setHasFocus(false);
        }
        if (terminal.getHasFocus() && installerWizard.getHasFocus()) {
            installerWizard.setHasFocus(false);
        }
        if (terminal.getHasFocus() && fileExplorer.getHasFocus()) {
            fileExplorer.setHasFocus(false);
        }
        if (installerWizard.getHasFocus() && fileExplorer.getHasFocus()) {
            fileExplorer.setHasFocus(false);
        }
    }
}

void Game::update() {
    desktop.update();
    notepad.update();
    terminal.update();
    installerWizard.update();
    fileExplorer.update();
    glitchManager.update();

    if (terminal.isRecoveryComplete()) {
        isDriveRecoveryCorrupted = true;
    }
    fileExplorer.setBasementDriveVisible(isDriveRecoveryCorrupted);
    
    // Handle file explorer item activation
    if (fileExplorer.isActivationRequested()) {
        fileExplorer.clearActivationRequest();
        std::string itemType = fileExplorer.getSelectedItemType();
        std::string itemName = fileExplorer.getSelectedItemName();
        std::string itemPath = fileExplorer.getSelectedItemPath();
        
        if (itemType == "txt") {
            // Hardcoded file contents for system files
            std::string content = "";
            if (itemName == "todo_list.txt") {
                content = "AMITY OS TASK LIST\n================\n\n1. Fix terminal buffer overflow\n2. Run drive recovery scan\n3. Patch memory leak in scheduler\n4. Investigate unauthorized access attempts";
            } else if (itemName == "system_log.txt") {
                content = "SYSTEM LOG - AMITY OS\n====================\n\n[WARNING] Sector 0x04F2 contains unidentified logic injection\n[ERROR] Authentication layer compromised\n[CRITICAL] Subsystem interference detected\n[INFO] Last boot: 23:41 GMT";
            } else if (itemName == "readme.md") {
                content = "# Amity Operating System\n\nWelcome to Amity OS - A revolutionary operating environment.\n\nThis file explorer serves as the hub for all system files and applications, much like a real Windows environment.\n\nDouble-click any item to open it with the appropriate application.";
            } else if (itemName == "basement_key.txt") {
                content = "BASEMENT ENCRYPTION KEY\n=======================\nKey: [F3-88B-A1]\n\nDo not share this key with anyone.";
            } else if (itemName == "encrypted_data.bin") {
                content = "01000100 01000101 01000011 01010010 01011001 01010000 01010100\n[ERROR: Buffer unaligned. Load decryption module abstractor]";
            } else if (itemName == "memories.txt") {
                content = "MEMORIES OF 1994\n================\n\nThis was the summer we built the computer in the basement.\nI still remember the hum of the CRT and the blinking drive light.";
            }
            
            notepad.openFile(itemName, content);
            notepad.setIsOpen(true);
            notepad.setHasFocus(true);
            fileExplorer.setHasFocus(false);
        } else if (itemType == "installer") {
            installerWizard.setIsOpen(true);
            installerWizard.setHasFocus(true);
            fileExplorer.setHasFocus(false);
            notepad.setHasFocus(false);
            terminal.setHasFocus(false);
        }
    }
}

void Game::render() {
    window.clear();
    
    desktop.draw(window);
    
    // draw windows in z-order (unfocused first, focused on top)
    if (installerWizard.getHasFocus() && installerWizard.getIsOpen()) {
        notepad.draw(window);
        terminal.draw(window);
        fileExplorer.draw(window);
        installerWizard.draw(window);
    } else if (fileExplorer.getHasFocus() && fileExplorer.getIsOpen()) {
        notepad.draw(window);
        terminal.draw(window);
        installerWizard.draw(window);
        fileExplorer.draw(window);
    } else if (terminal.getHasFocus() && terminal.getIsOpen()) {
        notepad.draw(window);
        installerWizard.draw(window);
        fileExplorer.draw(window);
        terminal.draw(window);
    } else if (notepad.getHasFocus() && notepad.getIsOpen()) {
        terminal.draw(window);
        installerWizard.draw(window);
        fileExplorer.draw(window);
        notepad.draw(window);
    } else {
        notepad.draw(window);
        installerWizard.draw(window);
        terminal.draw(window);
        fileExplorer.draw(window);
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