#include "apps/defragApp.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

DefragApp::DefragApp() : VirtualWindow("Memory Abstractor", 320, 260) {
    isOpen = false;
    isDefragmenting = false;
    progress = 0.f;
    cleanRequested = false;

    windowFrame.setFillColor(sf::Color(192, 192, 192));

    if (!font.loadFromFile("c:/WINDOWS/Fonts/CONSOLA.TTF")) {
        std::cerr << "[ERROR] DefragApp FAILED TO LOAD FONT\n";
    }

    statusHeader.setFont(font);
    statusHeader.setString("SYSTEM HEAP STATUS:");
    statusHeader.setCharacterSize(13);
    statusHeader.setFillColor(sf::Color(0, 0, 128));
    statusHeader.setStyle(sf::Text::Bold);

    statusInfo.setFont(font);
    statusInfo.setString("Heap Allocation: UNSTABLE (leak detected)");
    statusInfo.setCharacterSize(11);
    statusInfo.setFillColor(sf::Color::Red);

    defragBtn.setSize(sf::Vector2f(160.f, 30.f));
    defragBtn.setFillColor(sf::Color(220, 220, 220));
    defragBtn.setOutlineThickness(1.5f);
    defragBtn.setOutlineColor(sf::Color::Black);

    defragBtnText.setFont(font);
    defragBtnText.setString("Defragment Heap");
    defragBtnText.setCharacterSize(12);
    defragBtnText.setFillColor(sf::Color::Black);

    progressBarBg.setSize(sf::Vector2f(280.f, 22.f));
    progressBarBg.setFillColor(sf::Color(120, 120, 120));
    progressBarBg.setOutlineThickness(1.5f);
    progressBarBg.setOutlineColor(sf::Color::Black);

    progressBar.setSize(sf::Vector2f(0.f, 22.f));
    progressBar.setFillColor(sf::Color(0, 0, 128));

    progressText.setFont(font);
    progressText.setString("Idle");
    progressText.setCharacterSize(11);
    progressText.setFillColor(sf::Color::Black);
}

bool DefragApp::checkAndClearCleanRequest() {
    if (cleanRequested) {
        cleanRequested = false;
        return true;
    }
    return false;
}

void DefragApp::setLeakCount(int leaks) {
    leakCount = leaks;
}

void DefragApp::update() {
    VirtualWindow::update();

    if (isDefragmenting) {
        float elapsed = progressClock.getElapsedTime().asSeconds();
        progress = (elapsed / 8.f) * 100.f;

        if (progress >= 100.f) {
            progress = 100.f;
            isDefragmenting = false;
            cleanRequested = true;
            statusInfo.setString("Heap Status: OPTIMIZED");
            statusInfo.setFillColor(sf::Color(0, 128, 0));
        }
    } else {
        if (leakCount > 0) {
            statusInfo.setString("Heap Status: Fragmented (" + std::to_string(leakCount) + " leaks)");
            statusInfo.setFillColor(sf::Color::Red);
        } else {
            statusInfo.setString("Heap Status: OPTIMIZED");
            statusInfo.setFillColor(sf::Color(0, 128, 0));
        }
    }
}

void DefragApp::draw(sf::RenderWindow& window) {
    if (!isOpen) return;
    VirtualWindow::draw(window);
    if (getIsOpening()) return;

    sf::Vector2f winPos = windowFrame.getPosition();

    statusHeader.setPosition(winPos.x + 20.f, winPos.y + 40.f);
    statusInfo.setPosition(winPos.x + 20.f, winPos.y + 60.f);
    
    progressBarBg.setPosition(winPos.x + 20.f, winPos.y + 90.f);
    progressBar.setPosition(winPos.x + 20.f, winPos.y + 90.f);
    progressBar.setSize(sf::Vector2f((progress / 100.f) * 280.f, 22.f));

    progressText.setPosition(winPos.x + 20.f, winPos.y + 120.f);
    if (isDefragmenting) {
        std::stringstream ss;
        ss << "Optimizing memory... " << std::fixed << std::setprecision(1) << progress << "%";
        progressText.setString(ss.str());
        progressText.setFillColor(sf::Color(100, 0, 0));
    } else {
        if (progress >= 100.f) {
            progressText.setString("Defragmentation completed successfully.");
            progressText.setFillColor(sf::Color(0, 100, 0));
        } else {
            progressText.setString("Ready for alignment optimization.");
            progressText.setFillColor(sf::Color::Black);
        }
    }

    window.draw(statusHeader);
    window.draw(statusInfo);
    window.draw(progressBarBg);
    if (progress > 0.f) {
        window.draw(progressBar);
    }
    window.draw(progressText);

    if (!isDefragmenting) {
        defragBtn.setPosition(winPos.x + 80.f, winPos.y + 190.f);
        defragBtnText.setPosition(winPos.x + 95.f, winPos.y + 198.f);
        window.draw(defragBtn);
        window.draw(defragBtnText);
    }
}

void DefragApp::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    VirtualWindow::handleEvent(event, window);

    if (!isOpen) return;

    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF = window.mapPixelToCoords(pixelPos);

    sf::Vector2f winPos = windowFrame.getPosition();
    sf::FloatRect btnBounds(winPos.x + 80.f, winPos.y + 190.f, defragBtn.getSize().x, defragBtn.getSize().y);

    if (!isDefragmenting) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (btnBounds.contains(mousePosF)) {
                defragBtn.setFillColor(sf::Color(160, 160, 160));
                isDefragmenting = true;
                progress = 0.f;
                progressClock.restart();
                statusInfo.setString("Heap Allocation: RUNNING SCAN...");
                statusInfo.setFillColor(sf::Color(100, 100, 0));
            }
        } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            defragBtn.setFillColor(sf::Color(220, 220, 220));
        } else if (event.type == sf::Event::MouseMoved) {
            if (btnBounds.contains(mousePosF)) {
                defragBtn.setFillColor(sf::Color(200, 200, 200));
            } else {
                defragBtn.setFillColor(sf::Color(220, 220, 220));
            }
        }
    }
}
