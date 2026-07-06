#include <iostream>

#include "game.hpp"

Game::Game() {
    initWindow();
    desktop.init(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    windowManager.addWindow(&installerWizard);
    windowManager.addWindow(&fileExplorer);
    windowManager.addWindow(&terminal);
    terminal.setFileExplorer(&fileExplorer);
    windowManager.addWindow(&notepad);
    windowManager.addWindow(&settingsApp);
    windowManager.addWindow(&driveRecovery);
    windowManager.addWindow(&antivirusApp);
    windowManager.addWindow(&defragApp);

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
    cpuUtil = 10.f;
    ramUtil = 10.f;
    cpuTemp = 40.f;
    overheatTimer = 5.f;
    isOverheating = false;
    isInputFrozen = false;

    currentState = GameState::BootSequence;
    bootDuration = 8.0f;
    usbTriggerDelay = 25.0f;
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
        if (event.type == sf::Event::Resized) {
            updateWindowView(event.size.width, event.size.height);
        }

        if (isInputFrozen) {
            continue;
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

        if (currentState != GameState::NormalOS && currentState != GameState::ActiveOS && currentState != GameState::CorruptedOS) {
            continue; // Block events during boot sequence, BSOD, etc.
        }
        
        if (installerWizard.getIsOpen() && installerWizard.getIsErrorOpen()) {
            installerWizard.handleEvent(event, window);
            continue;
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            bool popupClicked = false;
            auto it = activePopups.begin();
            while (it != activePopups.end()) {
                sf::FloatRect bounds(it->closeButton.getPosition().x, it->closeButton.getPosition().y, it->closeButton.getSize().x, it->closeButton.getSize().y);
                if (bounds.contains(mousePos)) {
                    it = activePopups.erase(it);
                    popupClicked = true;
                    std::cout << "[OS Engine] Closed popup alert.\n";
                    break;
                }
                ++it;
            }
            if (popupClicked) {
                continue;
            }
        }

        bool desktopIconClicked = false;
        VirtualWindow* forcedFocusWindow = nullptr;
        std::string clickedApp = desktop.handleEvent(event, window);
        sf::Vector2f iconPos(-1.f, -1.f);
        
        if (!clickedApp.empty()) {
            desktopIconClicked = true;
            iconPos = desktop.getIconPosition(clickedApp);
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
            else if (clickedApp == "antivirus") {
                std::cout << "[OS Engine] Opening Antivirus...\n";
                antivirusApp.setIsOpen(true);
                forcedFocusWindow = &antivirusApp;
            }
            else if (clickedApp == "abstractor") {
                std::cout << "[OS Engine] Opening Memory Abstractor...\n";
                defragApp.setIsOpen(true);
                forcedFocusWindow = &defragApp;
            }
        }

        windowManager.processWindowEvents(event, window, desktopIconClicked, forcedFocusWindow, iconPos);
    }
}

void Game::update() {
    if (currentState == GameState::BootSequence) {
        float elapsed = stateClock.getElapsedTime().asSeconds();
        float progress = std::min(elapsed / bootDuration, 1.0f);
        bootProgressBar.setSize(sf::Vector2f(progress * 400.f, 20.f));

        if (isNextBootCorrupted) {
            std::string msg = "Booting AmityOS (Safe Mode)...";
            if (progress >= 0.70f) {
                for (size_t i = 0; i < msg.length(); ++i) {
                    if (msg[i] != ' ' && msg[i] != '(' && msg[i] != ')' && msg[i] != '.' && (rand() % 4 == 0)) {
                        char symbols[] = {'0', '!', '@', '#', '$', '%', '&', '*', '3', '7', '1', '?'};
                        msg[i] = symbols[rand() % 12];
                    }
                }
            }
            biosText.setString("AMITY SYSTEM DIAGNOSTICS\n"
                               "========================\n"
                               "CPU: Marrow 486 DX2\n"
                               "RAM: 640KB Conventional / 64MB Extended\n\n"
                               "Status: " + msg);
        } else {
            biosText.setString("AMITY OS BIOS v1.02\nCPU: Amity DX4 100MHz\nRAM: 640KB OK\nChecking IDE Drives...\nOK.\n\nLoading OS...");
        }
        
        if (elapsed > bootDuration) {
            notepad.setIsOpen(false);
            terminal.setIsOpen(false);
            installerWizard.setIsOpen(false);
            fileExplorer.setIsOpen(false);
            settingsApp.setIsOpen(false);
            defragApp.setIsOpen(false);
            antivirusApp.setIsOpen(false);
            driveRecovery.setIsOpen(false);

            if (isNextBootCorrupted) {
                currentState = GameState::CorruptedOS;
                desktop.setCorruptedTheme(true);
                installerWizard.setSystemCorrupted(true);
                driveRecovery.enterCorruptedMode();
                driveRecovery.setIsOpen(true);
                windowManager.bringToFront(&driveRecovery);
                isInputFrozen = false;
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

    if (driveRecovery.getIsRunning() && driveRecovery.getProgress() >= 99.f) {
        isInputFrozen = true;
        if (rand() % 15 == 0) {
            glitchManager.triggerJumpscare();
        }
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
        antivirusApp.setIsOpen(false);
        defragApp.setIsOpen(false);
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
            bootDuration = 6.0f;
            stateClock.restart();
        }
        return;
    }

    static sf::Clock dtClock;
    float dt = dtClock.restart().asSeconds();
    if (dt > 0.1f) dt = 0.1f;

    if (currentState == GameState::NormalOS || currentState == GameState::ActiveOS || currentState == GameState::CorruptedOS) {
        float activeRam = 10.f; // Baseline conventional memory
        if (terminal.getIsOpen()) activeRam += 12.f;
        if (fileExplorer.getIsOpen()) activeRam += 12.f;
        if (notepad.getIsOpen()) activeRam += 12.f;
        if (driveRecovery.getIsOpen()) activeRam += 12.f;
        if (settingsApp.getIsOpen()) activeRam += 12.f;
        if (antivirusApp.getIsOpen()) activeRam += 12.f;
        if (defragApp.getIsOpen()) activeRam += 12.f;
        if (installerWizard.getIsOpen()) activeRam += 12.f;

        if (antivirusApp.getIsMasterShieldActive()) {
            activeRam += 15.f; // Active background task RAM load
        }

        float leakCost = antivirusApp.isMemoryFirewallActive() ? 4.f : 8.f;
        activeRam += fileExplorer.countCorruptedFiles() * leakCost;

        ramUtil = activeRam;
        if (ramUtil >= 100.f) {
            ramUtil = 100.f;
            std::cout << "[OS Engine] RAM Overflow! Instant crash.\n";
            currentState = GameState::BSOD;
            bsodClock.restart();

            installerWizard.setIsOpen(false);
            fileExplorer.setIsOpen(false);
            terminal.setIsOpen(false);
            notepad.setIsOpen(false);
            settingsApp.setIsOpen(false);
            driveRecovery.setIsOpen(false);
            antivirusApp.setIsOpen(false);
            defragApp.setIsOpen(false);
            activePopups.clear();
            return;
        }

        // Configure static RAM lag multiplier
        VirtualWindow::ramLagMultiplier = 1.0f + (ramUtil / 10.f);

        float activeCpu = 10.f;
        if (terminal.getIsOpen()) activeCpu += 5.f;
        if (fileExplorer.getIsOpen()) activeCpu += 5.f;
        if (notepad.getIsOpen()) activeCpu += 2.f;
        if (driveRecovery.getIsOpen() && driveRecovery.getIsRunning()) activeCpu += 15.f;
        
        bool antivirusActive = installerWizard.getIsFinalized() && installerWizard.isComponentChecked("antivirus");
        bool defragActive = installerWizard.getIsFinalized() && installerWizard.isComponentChecked("abstractor");
        bool healthActive = installerWizard.getIsFinalized() && installerWizard.isComponentChecked("health_monitor");

        if (antivirusActive) {
            activeCpu += 5.f;
            if (antivirusApp.getIsMasterShieldActive()) {
                activeCpu += 15.f;
            }
            if (antivirusApp.isFileShieldActive() || antivirusApp.isActiveMonitorActive() || antivirusApp.isMemoryFirewallActive()) {
                activeCpu += 10.f;
            }
        }
        if (defragActive) {
            activeCpu += 5.f;
            if (defragApp.getIsDefragmenting()) {
                activeCpu += 30.f;
            }
        }

        activeCpu += activePopups.size() * 6.f;

        cpuUtil = activeCpu;
        if (cpuUtil > 100.f) cpuUtil = 100.f;

        float targetTemp = 40.f;
        if (cpuUtil <= 30.f) {
            targetTemp = 40.f;
        } else if (cpuUtil <= 70.f) {
            targetTemp = 40.f + ((cpuUtil - 30.f) / 40.f) * 50.f;
        } else {
            targetTemp = 90.f + ((cpuUtil - 70.f) / 30.f) * 20.f;
        }

        float heatSpeed = 0.08f;
        if (cpuUtil > 70.f) heatSpeed = 0.15f;
        if (cpuUtil >= 95.f) heatSpeed = 0.25f;

        float tempDiff = targetTemp - cpuTemp;
        cpuTemp += tempDiff * heatSpeed * dt;

        if (cpuTemp < 40.f) cpuTemp = 40.f;
        if (cpuTemp > 115.f) cpuTemp = 115.f;

        desktop.setMetrics(cpuUtil, ramUtil, cpuTemp, healthActive);

        updatePopups(dt);

        // Reconciliation of desktop shortcuts
        std::vector<std::string> vfsDesktopNames = fileExplorer.getDesktopFileNames();
        static std::vector<std::string> spawnedLeaks;
        auto leakIt = spawnedLeaks.begin();
        while (leakIt != spawnedLeaks.end()) {
            bool found = false;
            for (const auto& name : vfsDesktopNames) {
                if (name == *leakIt) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                desktop.removeIcon("file_" + *leakIt);
                leakIt = spawnedLeaks.erase(leakIt);
            } else {
                ++leakIt;
            }
        }

        // Terminal puzzle unlocks
        if (terminal.checkAndClearRegsync()) {
            driveRecovery.unlockSector(2);
            // Spawn sect1.txt so player gets confirmation
            fileExplorer.addFileToFolder("C:\\sys", "sect1.txt", "txt");
        }
        if (terminal.checkAndClearFat32()) {
            driveRecovery.unlockSector(3);
            // Spawn sect2.txt so player gets confirmation
            fileExplorer.addFileToFolder("C:\\sys", "sect2.txt", "txt");
        }
        if (driveRecovery.getCurrentSector() == 4 && driveRecovery.isSectorLocked() && defragApp.checkAndClearCleanRequest()) {
            driveRecovery.unlockSector(4);
            // Allow cleaning memory leaks as normal
            std::cout << "[OS Kernel] Memory Abstractor optimized heap. Cleaning memory leaks...\n";
            fileExplorer.clearCorruptedFiles();
        }
        if (terminal.checkAndClearSafe()) {
            driveRecovery.unlockSector(5);
        }

        // Sector Checkpoints & Wave triggers
        if (driveRecovery.isWaveActive() && currentWave == 0) {
            float progress = driveRecovery.getProgress();
            if (progress >= 20.f && progress < 22.f) {
                currentWave = 1;
                waveTimer = 25.f;
                popupSpawnTimer = 0.5f;
                fileSpawnTimer = 1.0f;
                std::cout << "[Virus Wave] Wave 1 started!\n";
            } else if (progress >= 40.f && progress < 42.f) {
                currentWave = 2;
                waveTimer = 25.f;
                popupSpawnTimer = 0.5f;
                fileSpawnTimer = 1.0f;
                std::cout << "[Virus Wave] Wave 2 started!\n";
            } else if (progress >= 60.f && progress < 62.f) {
                currentWave = 3;
                waveTimer = 25.f;
                popupSpawnTimer = 0.5f;
                fileSpawnTimer = 1.0f;
                std::cout << "[Virus Wave] Wave 3 started!\n";
            } else if (progress >= 80.f && progress < 82.f) {
                currentWave = 4;
                waveTimer = 25.f;
                popupSpawnTimer = 0.5f;
                fileSpawnTimer = 1.0f;
                std::cout << "[Virus Wave] Wave 4 started!\n";
            } else if (progress >= 100.f) {
                currentWave = 5;
                waveTimer = 25.f;
                popupSpawnTimer = 0.5f;
                fileSpawnTimer = 1.0f;
                std::cout << "[Virus Wave] Final Wave 5 started!\n";
            }
        }

        if (currentWave > 0) {
            waveTimer -= dt;
            popupSpawnTimer -= dt;
            fileSpawnTimer -= dt;

            float popupInterval = 3.5f;
            if (currentWave == 2) popupInterval = 2.0f;
            if (currentWave == 3) popupInterval = 1.5f;
            if (currentWave == 4) popupInterval = 1.0f;
            if (currentWave == 5) popupInterval = 0.8f;

            if (antivirusApp.isActiveMonitorActive()) {
                popupInterval *= 2.0f; 
            }

            if (popupSpawnTimer <= 0.f) {
                spawnPopup();
                popupSpawnTimer = popupInterval;
            }

            float fileInterval = 4.5f;
            if (currentWave == 2) fileInterval = 3.0f;
            if (currentWave == 3) fileInterval = 2.0f;
            if (currentWave == 4) fileInterval = 1.5f;
            if (currentWave == 5) fileInterval = 1.0f;

            if (fileSpawnTimer <= 0.f) {
                if (antivirusApp.isFileShieldActive()) {
                    std::cout << "[Antivirus] Passively blocked a file leak spawn attempt!\n";
                    antivirusApp.logBlockedThreat("File Leak Threat (Filesystem Guard)");
                    fileSpawnTimer = fileInterval;
                } else {
                    static int leakIndex = 0;
                    std::string filename = "leak_" + std::to_string(leakIndex++) + ".sys";
                    fileExplorer.addFileToFolder("C:\\Desktop", filename, "txt");
                    desktop.createIcon(filename, "file_" + filename);
                    spawnedLeaks.push_back(filename);
                    std::cout << "[Virus Wave] Created corrupted file: " << filename << " on desktop\n";
                    fileSpawnTimer = fileInterval;
                }
            }

            if (waveTimer <= 0.f) {
                std::cout << "[Virus Wave] Wave " << currentWave << " survived!\n";
                activePopups.clear();

                if (currentWave == 1) {
                    driveRecovery.triggerNextWave(1, 40.f);
                } else if (currentWave == 2) {
                    driveRecovery.triggerNextWave(2, 60.f);
                } else if (currentWave == 3) {
                    driveRecovery.triggerNextWave(3, 80.f);
                } else if (currentWave == 4) {
                    driveRecovery.triggerNextWave(4, 100.f);
                } else if (currentWave == 5) {
                    driveRecovery.setWaveActive(false);
                    std::cout << "[OS Engine] Sector 5 scanned! Diagnostics complete!\n";
                }
                currentWave = 0;
            }
        }

        if (cpuTemp >= 108.f) {
            isOverheating = true;
            overheatTimer -= dt;
            if (rand() % 30 == 0) {
                glitchManager.triggerJumpscare();
            }
            if (overheatTimer <= 0.f) {
                std::cout << "[OS Engine] CPU Overheat! System crashed.\n";
                cpuTemp = 40.f;
                cpuUtil = 10.f;
                ramUtil = 10.f;
                overheatTimer = 5.f;
                isOverheating = false;
                currentState = GameState::BootSequence;
                bootDuration = 4.0f;
                stateClock.restart();

                float currentProg = driveRecovery.getProgress();
                driveRecovery.setProgress(std::max(0.f, currentProg - 10.f));
                driveRecovery.setWaveActive(false);
            }
        } else {
            isOverheating = false;
            overheatTimer = 5.f;
        }
    }

    if (currentState == GameState::NormalOS) {
        if (systemLogsSpawnTimer < 0.f) {
            systemLogsSpawnTimer = 15.f;
        }
        if (systemLogsSpawnTimer > 0.f) {
            systemLogsSpawnTimer -= dt;
            if (systemLogsSpawnTimer <= 0.f) {
                fileExplorer.addFileToFolder("C:\\Desktop", "system_log.txt", "txt");
                desktop.createIcon("system_log.txt", "file_system_log.txt");
                std::cout << "[OS Kernel] Spawned system_log.txt on desktop after mount delay.\n";
            }
        }

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
    antivirusApp.update();
    defragApp.setLeakCount(fileExplorer.countCorruptedFiles());
    defragApp.update();
    glitchManager.update();

    if (defragApp.checkAndClearCleanRequest()) {
        std::cout << "[OS Kernel] Memory Abstractor optimized heap. Cleaning memory leaks...\n";
        fileExplorer.clearCorruptedFiles();
    }

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
                content = "MEMORIES OF 1994\n================\n\nThis was the summer we built the computer in the basement.\nI still remember the hum of the CRT and the blinking drive light.\nMarrow computers were built to last.\nMarrow is everything. Marrow represents conventional abstractors.\nWe are marrow.";
            } else if (itemName == "sect1.txt") {
                content = "SECTOR 1 DIAGNOSTIC LOG\n=======================\nRegistry mapping mismatched in Sector 2.\n\nTo align registry allocations, type 'regsync' inside the Terminal.";
            } else if (itemName == "sect2.txt") {
                content = "SECTOR 2 DIAGNOSTIC LOG\n=======================\nFAT32 checksum mismatch in Sector 3.\n\nUnlock command parameters: unlock FAT32 <key>.\nThe key is the case-insensitive occurrence count of the word 'marrow' in the file memories.txt in basement drive D:.";
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
        if (installerWizard.isComponentChecked("antivirus")) {
            desktop.createIcon("Amity Shield", "antivirus");
        }
        if (installerWizard.isComponentChecked("abstractor")) {
            desktop.createIcon("Mem Abstractor", "abstractor");
        }
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
    drawPopups();
    
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
        window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "totallynotavirus", sf::Style::Titlebar | sf::Style::Close);
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

void Game::spawnPopup() {
    bool antivirusActive = installerWizard.getIsFinalized() && installerWizard.isComponentChecked("antivirus");
    if (antivirusActive && antivirusApp.isActiveMonitorActive()) {
        std::cout << "[Antivirus] Passively blocked a virus popup spawn attempt!\n";
        antivirusApp.logBlockedThreat("Popup Alert Threat (Active Monitor)");
        return;
    }

    float rx = 50.f + static_cast<float>(rand() % 650);
    float ry = 50.f + static_cast<float>(rand() % 500);
    sf::Vector2f vel(0.f, 0.f);
    if (currentWave >= 2) {
        vel.x = -120.f + static_cast<float>(rand() % 240);
        vel.y = -120.f + static_cast<float>(rand() % 240);
        if (abs(vel.x) < 20.f) vel.x = 60.f;
        if (abs(vel.y) < 20.f) vel.y = 60.f;
    }
    activePopups.push_back(PopupAlert(systemFont, rx, ry, vel));
    std::cout << "[Virus Wave] Spawned popup alert.\n";
}

void Game::updatePopups(float dt) {
    for (auto& popup : activePopups) {
        if (currentWave >= 2) {
            sf::Vector2f pos = popup.frame.getPosition();
            pos += popup.velocity * dt;

            // Bounce off screen boundaries
            if (pos.x < 0.f) {
                pos.x = 0.f;
                popup.velocity.x = -popup.velocity.x;
            }
            if (pos.x + popup.frame.getSize().x > 1024.f) {
                pos.x = 1024.f - popup.frame.getSize().x;
                popup.velocity.x = -popup.velocity.x;
            }
            if (pos.y < 0.f) {
                pos.y = 0.f;
                popup.velocity.y = -popup.velocity.y;
            }
            if (pos.y + popup.frame.getSize().y > 768.f - 40.f) {
                pos.y = 768.f - 40.f - popup.frame.getSize().y;
                popup.velocity.y = -popup.velocity.y;
            }
            popup.setPosition(pos.x, pos.y);
        }
    }
}

void Game::drawPopups() {
    for (const auto& popup : activePopups) {
        window.draw(popup.frame);
        window.draw(popup.titleBar);
        window.draw(popup.closeButton);
        window.draw(popup.titleText);
        window.draw(popup.bodyText);
        window.draw(popup.closeText);
    }
}