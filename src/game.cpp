#include <iostream>

#include "game.hpp"

Game::Game() {
    initWindow();
    desktop.init(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    windowManager.addWindow(&installerWizard);
    windowManager.addWindow(&fileExplorer);
    windowManager.addWindow(&terminal);
    windowManager.addWindow(&notepad);
    windowManager.addWindow(&settingsApp);
    windowManager.addWindow(&driveRecovery);

    // BSOD UI Setup
    bsodBg.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    bsodBg.setFillColor(sf::Color(0, 0, 150));
    bsodBg.setPosition(0, 0);

    bsodText.setFont(systemFont);
    bsodText.setCharacterSize(14);
    bsodText.setFillColor(sf::Color::White);
    bsodText.setPosition(40.f, 40.f);
    bsodText.setString(
        "A problem has been detected and Amity OS has been shut down to prevent damage\n"
        "to your computer.\n\n"
        "UNEXPECTED_KERNEL_MODE_TRAP\n\n"
        "If this is the first time you've seen this Stop error screen,\n"
        "restart your computer. If this screen appears again, follow\n"
        "these steps:\n\n"
        "Check to make sure any new hardware or software is properly installed.\n"
        "If this is a new installation, ask your hardware or software manufacturer\n"
        "for any Amity OS updates you might need.\n\n"
        "Technical information:\n"
        "*** STOP: 0x0000007F (0x00000008, 0x00000000, 0x00000000, 0x00000000)\n"
        "*** amitydisk.sys - Address F892A203 base at F8929000, DateStamp 36b072a2"
    );

    systemDelayMultiplier = 1.0f;

    currentState = GameState::BootSequence;
    bootDuration = 5.0f;
    usbTriggerDelay = 10.0f;
    usbPluggedIn = false;
    
    if (!systemFont.loadFromFile("c:/WINDOWS/Fonts/CONSOLA.TTF")) {
        std::cerr << "[ERROR] Game FAILED TO LOAD FONT\n";
    }

    bsodText.setFont(systemFont);

    // Boot UI
    biosText.setFont(systemFont);
    biosText.setCharacterSize(14);
    biosText.setFillColor(sf::Color::White);
    biosText.setPosition(10.f, 10.f);
    biosText.setString("AMITY OS BIOS v1.02\nCPU: Amity DX4 100MHz\nRAM: 640KB OK\nChecking IDE Drives...\nOK.\n\nLoading OS...");

    bootProgressBarBackground.setSize(sf::Vector2f(400.f, 20.f));
    bootProgressBarBackground.setFillColor(sf::Color(100, 100, 100));
    bootProgressBarBackground.setPosition((SCREEN_WIDTH - 400.f) / 2.f, SCREEN_HEIGHT - 100.f);
    bootProgressBarBackground.setOutlineThickness(2.f);
    bootProgressBarBackground.setOutlineColor(sf::Color::White);

    bootProgressBar.setSize(sf::Vector2f(0.f, 20.f));
    bootProgressBar.setFillColor(sf::Color(0, 0, 128));
    bootProgressBar.setPosition(bootProgressBarBackground.getPosition());

    // USB Popup UI
    usbPopupFrame.setSize(sf::Vector2f(350.f, 150.f));
    usbPopupFrame.setFillColor(sf::Color(192, 192, 192));
    usbPopupFrame.setOutlineThickness(2.f);
    usbPopupFrame.setOutlineColor(sf::Color::White);
    usbPopupFrame.setPosition((SCREEN_WIDTH - 350.f) / 2.f, (SCREEN_HEIGHT - 150.f) / 2.f);

    usbPopupTitleBar.setSize(sf::Vector2f(350.f, 22.f));
    usbPopupTitleBar.setFillColor(sf::Color(0, 0, 128));
    usbPopupTitleBar.setPosition(usbPopupFrame.getPosition());

    usbPopupTitleText.setFont(systemFont);
    usbPopupTitleText.setString("New Hardware Detected");
    usbPopupTitleText.setCharacterSize(12);
    usbPopupTitleText.setFillColor(sf::Color::White);
    usbPopupTitleText.setStyle(sf::Text::Bold);
    usbPopupTitleText.setPosition(usbPopupTitleBar.getPosition().x + 5.f, usbPopupTitleBar.getPosition().y + 3.f);

    usbPopupBodyText.setFont(systemFont);
    usbPopupBodyText.setString("Removable Disk (D:) has been attached.\nDo you wish to initialize and mount the drive?");
    usbPopupBodyText.setCharacterSize(11);
    usbPopupBodyText.setFillColor(sf::Color::Black);
    usbPopupBodyText.setPosition(usbPopupFrame.getPosition().x + 10.f, usbPopupFrame.getPosition().y + 40.f);

    usbAcceptButton.setSize(sf::Vector2f(100.f, 25.f));
    usbAcceptButton.setFillColor(sf::Color(192, 192, 192));
    usbAcceptButton.setOutlineThickness(1.5f);
    usbAcceptButton.setOutlineColor(sf::Color(100, 100, 100));
    usbAcceptButton.setPosition(usbPopupFrame.getPosition().x + 50.f, usbPopupFrame.getPosition().y + 100.f);

    usbAcceptText.setFont(systemFont);
    usbAcceptText.setString("Mount");
    usbAcceptText.setCharacterSize(12);
    usbAcceptText.setFillColor(sf::Color::Black);
    usbAcceptText.setPosition(usbAcceptButton.getPosition().x + 30.f, usbAcceptButton.getPosition().y + 5.f);

    usbCancelButton.setSize(sf::Vector2f(100.f, 25.f));
    usbCancelButton.setFillColor(sf::Color(192, 192, 192));
    usbCancelButton.setOutlineThickness(1.5f);
    usbCancelButton.setOutlineColor(sf::Color(100, 100, 100));
    usbCancelButton.setPosition(usbPopupFrame.getPosition().x + 200.f, usbPopupFrame.getPosition().y + 100.f);

    usbCancelText.setFont(systemFont);
    usbCancelText.setString("Cancel");
    usbCancelText.setCharacterSize(12);
    usbCancelText.setFillColor(sf::Color::Black);
    usbCancelText.setPosition(usbCancelButton.getPosition().x + 28.f, usbCancelButton.getPosition().y + 5.f);

    stateClock.restart();
}

void Game::initWindow() {
    window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "totallynotavirus", sf::Style::Default);
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
        if (event.type == sf::Event::Resized) {
            updateWindowView(event.size.width, event.size.height);
        }

        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

        if (currentState == GameState::HardwarePrompt) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (usbAcceptButton.getGlobalBounds().contains(mousePos)) {
                    currentState = GameState::ActiveOS;
                } else if (usbCancelButton.getGlobalBounds().contains(mousePos)) {
                    currentState = GameState::HardwareCancelSequence;
                    usbPopupTitleText.setString("FATAL ERROR");
                    usbPopupBodyText.setString("Cannot unmount. Device is already corrupted.\nSystem stability compromised.");
                    usbPopupBodyText.setFillColor(sf::Color::Red);
                    stateClock.restart();
                }
            }
            continue; // Block all other events during hardware prompt
        }

        if (currentState != GameState::NormalOS && currentState != GameState::ActiveOS) {
            continue; // Block events during boot sequence
        }
        
        if (installerWizard.getIsOpen() && installerWizard.getIsErrorOpen()) {
            installerWizard.handleEvent(event, window);
            continue;
        }

        bool desktopIconClicked = false;
        VirtualWindow* forcedFocusWindow = nullptr;
        std::string clickedApp = desktop.handleEvent(event, window);
        
        if (!clickedApp.empty()) {
            desktopIconClicked = true;
            if (clickedApp == "txt_todo") {
                notepad.openFile("todo_list.txt", 
                    "AMITY OS SYSTEM LOG - TODO LIST\n"
                    "===================================\n"
                    "1. Fix terminal installation errors.\n"
                    "2. Run the Drive Recovery Tool.\n"
                    "3. DO NOT look at the basement drive light.");
                forcedFocusWindow = &notepad;
            }
            else if (clickedApp == "txt_log") {
                notepad.openFile("system_log.txt", 
                    "FATAL ERROR: Removable Drive (D:) corrupted.\n"
                    "Unidentified logic injection detected in flash device.\n\n"
                    "To initiate manual recovery, locate the backup manual in:\n"
                    "C:\\sys\\drivers");
                forcedFocusWindow = &notepad;
            }
            else if (clickedApp == "cmd") {
                forcedFocusWindow = &terminal;
            }
            else if (clickedApp == "pkg_installer") {
                forcedFocusWindow = &installerWizard;
            }
            else if (clickedApp == "file_explorer") {
                forcedFocusWindow = &fileExplorer;
            }
            else if (clickedApp == "shutdown_system") {
                std::cout << "[OS Engine] Shutting down system...\n";
                window.close();
            }
            else if (clickedApp == "open_control_panel") {
                std::cout << "[OS Engine] Opening Control Panel...\n";
                settingsApp.setIsOpen(true);
                forcedFocusWindow = &settingsApp;
            }
            else if (clickedApp == "drive_recovery") {
                std::cout << "[OS Engine] Opening Drive Recovery App...\n";
                driveRecovery.setIsOpen(true);
                forcedFocusWindow = &driveRecovery;
            }
        }

        windowManager.processWindowEvents(event, window, desktopIconClicked, forcedFocusWindow);
    }
}

void Game::update() {
    if (currentState == GameState::BootSequence) {
        float elapsed = stateClock.getElapsedTime().asSeconds();
        float progress = std::min(elapsed / bootDuration, 1.0f);
        bootProgressBar.setSize(sf::Vector2f(progress * 400.f, 20.f));
        
        if (elapsed > bootDuration) {
            if (isNextBootCorrupted) {
                currentState = GameState::CorruptedOS;
                desktop.setCorruptedTheme(true);
                driveRecovery.enterCorruptedMode();
                driveRecovery.setIsOpen(true);
                windowManager.bringToFront(&driveRecovery);
            } else {
                currentState = GameState::NormalOS;
            }
            stateClock.restart();
        }
        return;
    }

    if (settingsApp.isFullscreenToggleRequested()) {
        settingsApp.clearFullscreenToggleRequest();
        toggleFullscreen();
    }

    if (driveRecovery.isBsodTriggered()) {
        driveRecovery.clearBsodTriggered();
        currentState = GameState::BSOD;
        bsodClock.restart();
        
        installerWizard.setIsOpen(false);
        fileExplorer.setIsOpen(false);
        terminal.setIsOpen(false);
        notepad.setIsOpen(false);
        settingsApp.setIsOpen(false);
        driveRecovery.setIsOpen(false);
    }

    if (currentState == GameState::BSOD) {
        float elapsed = bsodClock.getElapsedTime().asSeconds();
        if (elapsed > 2.0f && rand() % 5 == 0) {
            std::string str = bsodText.getString();
            if (!str.empty()) {
                size_t index = rand() % str.length();
                if (str[index] != '\n' && str[index] != ' ') {
                    str[index] = static_cast<char>(33 + (rand() % 90));
                    bsodText.setString(str);
                }
            }
        }
        if (elapsed > bsodDuration) {
            isNextBootCorrupted = true;
            currentState = GameState::BootSequence;
            bootDuration = 3.0f;
            stateClock.restart();
        }
        return;
    }

    if (currentState == GameState::NormalOS) {
        if (!usbPluggedIn && stateClock.getElapsedTime().asSeconds() > usbTriggerDelay) {
            currentState = GameState::HardwarePrompt;
            usbPluggedIn = true;
        }
    }

    if (currentState == GameState::HardwareCancelSequence) {
        if (stateClock.getElapsedTime().asSeconds() > 2.5f) {
            window.close();
        }
    }

    desktop.update();
    notepad.update();
    terminal.update();
    installerWizard.update();
    fileExplorer.update();
    driveRecovery.update();
    glitchManager.update();

    if (terminal.isRecoveryComplete()) {
        isDriveRecoveryCorrupted = true;
    }
    fileExplorer.setBasementDriveVisible(isDriveRecoveryCorrupted);
    
    if (terminal.isInstallerDownloaded() && !hasSpawnedInstaller) {
        desktop.createIcon("App Installer", "pkg_installer");
        fileExplorer.addFileToDesktop("amityappsetup.exe", "installer");
        hasSpawnedInstaller = true;
    }
    
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
                content = "FATAL ERROR: Removable Drive (D:) corrupted.\nUnidentified logic injection detected in flash device.\n\nTo initiate manual recovery, locate the backup manual in:\nC:\\sys\\drivers";
            } else if (itemName == "recovery_guide.txt") {
                content = "AMITY DRIVE RECOVERY GUIDE\n==========================\n\nTo restore lost sectors, you must install the Amity Recovery Wizard.\nOpen the terminal and run the following package manager command:\n\npkgget install recovery-wizard";
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
            windowManager.bringToFront(&notepad);
        } else if (itemType == "installer") {
            windowManager.bringToFront(&installerWizard);
        }
    }
    
    if (installerWizard.getIsFinalized() && !hasRecalculatedPerformance) {
        recalculateSystemPerformance();
        desktop.createIcon("Drive Recovery", "drive_recovery");
        hasRecalculatedPerformance = true;
    }
}

void Game::render() {
    window.clear();
    
    if (currentState == GameState::BootSequence) {
        window.draw(biosText);
        window.draw(bootProgressBarBackground);
        window.draw(bootProgressBar);
        glitchManager.applyEffect(window);
        window.display();
        return;
    }

    if (currentState == GameState::BSOD) {
        window.draw(bsodBg);
        window.draw(bsodText);
        glitchManager.applyEffect(window);
        window.display();
        return;
    }

    desktop.draw(window);
    
    // draw windows in z-order (unfocused first, focused on top)
    windowManager.drawWindows(window);
    
    desktop.drawStartMenu(window);
    
    if (currentState == GameState::HardwarePrompt || currentState == GameState::HardwareCancelSequence) {
        window.draw(usbPopupFrame);
        window.draw(usbPopupTitleBar);
        window.draw(usbPopupTitleText);
        window.draw(usbPopupBodyText);
        
        if (currentState == GameState::HardwarePrompt) {
            window.draw(usbAcceptButton);
            window.draw(usbAcceptText);
            window.draw(usbCancelButton);
            window.draw(usbCancelText);
        }
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

void Game::toggleFullscreen() {
    isFullscreen = !isFullscreen;
    window.close();
    if (isFullscreen) {
        window.create(sf::VideoMode::getDesktopMode(), "totallynotavirus", sf::Style::None);
    } else {
        window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "totallynotavirus", sf::Style::Default);
    }
    window.setFramerateLimit(60);
    updateWindowView(window.getSize().x, window.getSize().y);
}

void Game::updateWindowView(unsigned int windowWidth, unsigned int windowHeight) {
    float targetAspectRatio = 1024.f / 768.f;
    float windowAspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
    
    sf::View view(sf::FloatRect(0.f, 0.f, 1024.f, 768.f));
    float vpWidth, vpHeight, vpLeft, vpTop;
    
    if (windowAspectRatio > targetAspectRatio) {
        vpWidth = targetAspectRatio / windowAspectRatio;
        vpHeight = 1.f;
        vpLeft = (1.f - vpWidth) / 2.f;
        vpTop = 0.f;
    } else {
        vpWidth = 1.f;
        vpHeight = windowAspectRatio / targetAspectRatio;
        vpLeft = 0.f;
        vpTop = (1.f - vpHeight) / 2.f;
    }
    
    view.setViewport(sf::FloatRect(vpLeft, vpTop, vpWidth, vpHeight));
    window.setView(view);
}