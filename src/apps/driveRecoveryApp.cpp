#include "apps/driveRecoveryApp.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

DriveRecoveryApp::DriveRecoveryApp() : VirtualWindow("Drive Recovery App", 400, 320) {
    isOpen = false;
    isRunning = false;
    progress = 0.f;
    bsodTriggered = false;
    corruptedMode = false;
    waveActive = false;
    currentWave = 0;
    nextCheckpoint = 33.f;

    windowFrame.setFillColor(sf::Color(192, 192, 192));

    if (!font.loadFromFile("c:/WINDOWS/Fonts/CONSOLA.TTF")) {
        std::cerr << "[ERROR] DriveRecoveryApp FAILED TO LOAD FONT\n";
    }

    startBtn.setSize(sf::Vector2f(160.f, 30.f));
    startBtn.setFillColor(sf::Color(220, 220, 220));
    startBtn.setOutlineThickness(1.5f);
    startBtn.setOutlineColor(sf::Color::Black);

    startBtnText.setFont(font);
    startBtnText.setString("Start Diagnostics");
    startBtnText.setCharacterSize(12);
    startBtnText.setFillColor(sf::Color::Black);

    progressBarBg.setSize(sf::Vector2f(360.f, 25.f));
    progressBarBg.setFillColor(sf::Color(120, 120, 120));
    progressBarBg.setOutlineThickness(1.5f);
    progressBarBg.setOutlineColor(sf::Color::Black);

    progressBar.setSize(sf::Vector2f(0.f, 25.f));
    progressBar.setFillColor(sf::Color(0, 0, 128));

    progressPercentText.setFont(font);
    progressPercentText.setString("0%");
    progressPercentText.setCharacterSize(12);
    progressPercentText.setFillColor(sf::Color::White);

    logBoxBg.setSize(sf::Vector2f(360.f, 130.f));
    logBoxBg.setFillColor(sf::Color(0, 0, 0));
    logBoxBg.setOutlineThickness(1.5f);
    logBoxBg.setOutlineColor(sf::Color::Black);

    logText.setFont(font);
    logText.setCharacterSize(10);
    logText.setFillColor(sf::Color(0, 255, 0));
    logText.setString("Amity Recovery Utility v1.0\nReady for diagnostics...");
}

void DriveRecoveryApp::addLog(const std::string& message) {
    logLines.push_back(message);
    if (logLines.size() > 9) {
        logLines.erase(logLines.begin());
    }
    updateLogText();
}

void DriveRecoveryApp::updateLogText() {
    std::stringstream ss;
    for (const auto& line : logLines) {
        ss << line << "\n";
    }
    logText.setString(ss.str());
}

void DriveRecoveryApp::startDiagnostics() {
    isRunning = true;
    progress = 0.f;
    logLines.clear();
    addLog("[INFO] Initializing sector scan...");
    addLog("[INFO] Accessing Removable Drive (D:)...");
    progressClock.restart();
}

void DriveRecoveryApp::enterCorruptedMode() {
    corruptedMode = true;
    isRunning = true; // Auto-resume recovery in corrupted mode
    progress = 0.f;
    currentWave = 0;
    nextCheckpoint = 33.f;
    waveActive = false;
    logLines.clear();
    addLog("[SYSTEM RECOVERY STATE]");
    addLog("[INFO] Reconstructing File Allocation Table...");
    progressClock.restart();
}

void DriveRecoveryApp::triggerNextWave(int waveNum, float checkpoint) {
    currentWave = waveNum;
    nextCheckpoint = checkpoint;
    waveActive = false;
    isRunning = true; // Resume progress
    progressClock.restart();
    addLog("[INFO] Lock released. Re-scanning segments...");
}

void DriveRecoveryApp::update() {
    VirtualWindow::update();

    if (isRunning) {
        if (!corruptedMode) {
            float elapsed = progressClock.getElapsedTime().asSeconds();
            float newProgress = (elapsed / 35.f) * 100.f;

            if (newProgress > progress) {
                int oldPercent = static_cast<int>(progress);
                int newPercent = static_cast<int>(newProgress);
                progress = newProgress;

                if (progress >= 99.f) {
                    progress = 99.f;
                    if (elapsed >= 43.f) { // 35s to hit 99%, 8s freeze
                        bsodTriggered = true;
                        isRunning = false;
                    }
                }

                for (int p = oldPercent + 1; p <= newPercent; ++p) {
                    if (p == 10) addLog("[INFO] Mount table verified.");
                    else if (p == 30) addLog("[INFO] Scanning boot record...");
                    else if (p == 50) addLog("[INFO] Indexing directories...");
                    else if (p == 75) addLog("[INFO] Reading sector 0xAB82...");
                    else if (p == 90) addLog("[WARN] Weak integrity sector at 0xCC10.");
                    else if (p == 98) addLog("[WARN] Reading sector 0xFFF0 (DANGER).");
                }
            }
        } else {
            // Corrupted wave mode
            if (!waveActive) {
                float dt = progressClock.restart().asSeconds();
                progress += dt * 1.5f; // Climb slowly
                
                if (progress >= nextCheckpoint) {
                    progress = nextCheckpoint;
                    waveActive = true;
                    isRunning = false; // Stop climbing, wave is active
                    addLog("[ALERT] Sectors locked by active intrusion!");
                    addLog("[ALERT] Contain threat to unlock sectors.");
                }
                
                if (progress >= 100.f) {
                    progress = 100.f;
                    isRunning = false;
                    addLog("[SUCCESS] Recovery complete. Drive mapping updated.");
                }
            }
        }
    }
}

void DriveRecoveryApp::draw(sf::RenderWindow& window) {
    if (!isOpen) return;
    VirtualWindow::draw(window);

    sf::Vector2f winPos = windowFrame.getPosition();

    // Position buttons & progress bar relative to window position
    progressBarBg.setPosition(winPos.x + 20.f, winPos.y + 40.f);
    progressBar.setPosition(winPos.x + 20.f, winPos.y + 40.f);
    progressBar.setSize(sf::Vector2f((progress / 100.f) * 360.f, 25.f));
    
    progressPercentText.setPosition(winPos.x + 180.f, winPos.y + 45.f);
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << progress << "%";
    progressPercentText.setString(ss.str());

    logBoxBg.setPosition(winPos.x + 20.f, winPos.y + 80.f);
    logText.setPosition(winPos.x + 25.f, winPos.y + 85.f);

    window.draw(progressBarBg);
    if (progress > 0.f) {
        window.draw(progressBar);
    }
    window.draw(progressPercentText);
    window.draw(logBoxBg);
    window.draw(logText);

    if (!isRunning && !corruptedMode && progress < 99.f) {
        startBtn.setPosition(winPos.x + 120.f, winPos.y + 230.f);
        startBtnText.setPosition(winPos.x + 140.f, winPos.y + 238.f);
        window.draw(startBtn);
        window.draw(startBtnText);
    }
}

void DriveRecoveryApp::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    VirtualWindow::handleEvent(event, window);

    if (!isOpen) return;

    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF = window.mapPixelToCoords(pixelPos);

    sf::Vector2f winPos = windowFrame.getPosition();
    sf::FloatRect btnBounds(winPos.x + 120.f, winPos.y + 230.f, startBtn.getSize().x, startBtn.getSize().y);

    if (!isRunning && !corruptedMode && progress < 99.f) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (btnBounds.contains(mousePosF)) {
                startBtn.setFillColor(sf::Color(160, 160, 160));
                startDiagnostics();
            }
        } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            startBtn.setFillColor(sf::Color(220, 220, 220));
        } else if (event.type == sf::Event::MouseMoved) {
            if (btnBounds.contains(mousePosF)) {
                startBtn.setFillColor(sf::Color(200, 200, 200));
            } else {
                startBtn.setFillColor(sf::Color(220, 220, 220));
            }
        }
    }
}
