#include "apps/antivirusApp.hpp"
#include <iostream>

AntivirusApp::AntivirusApp() : VirtualWindow("Amity Shield", 400, 400) {
    isOpen = false;
    windowFrame.setFillColor(sf::Color(192, 192, 192));

    if (!font.loadFromFile("c:/WINDOWS/Fonts/CONSOLA.TTF")) {
        std::cerr << "[ERROR] AntivirusApp FAILED TO LOAD FONT\n";
    }

    statusHeader.setFont(font);
    statusHeader.setString("AMITY SHIELD ACTIVE GUARD - OFF");
    statusHeader.setCharacterSize(12);
    statusHeader.setFillColor(sf::Color(100, 100, 100));
    statusHeader.setStyle(sf::Text::Bold);

    descText.setFont(font);
    descText.setString("Active shields suppress minor glitches,\nbut consume base conventional CPU cycle load.");
    descText.setCharacterSize(10);
    descText.setFillColor(sf::Color::Black);

    createCheckbox("Filesystem Guard (popups)", "file_shield", 30.f, 105.f);
    createCheckbox("Active Monitor (scripts)", "active_shield", 30.f, 140.f);
    createCheckbox("Memory Firewall (corruption)", "mem_shield", 30.f, 175.f);
    if (!checkboxes.empty()) {
        checkboxes[0].isChecked = true;
    }

    toggleBtn.setSize(sf::Vector2f(90.f, 25.f));
    toggleBtn.setFillColor(sf::Color(100, 200, 100));
    toggleBtn.setOutlineThickness(1.5f);
    toggleBtn.setOutlineColor(sf::Color::White);

    toggleBtnText.setFont(font);
    toggleBtnText.setString("TURN ON");
    toggleBtnText.setCharacterSize(11);
    toggleBtnText.setFillColor(sf::Color::Black);
    toggleBtnText.setStyle(sf::Text::Bold);

    logBoxTitleText.setFont(font);
    logBoxTitleText.setString("Blocked Threats Log:");
    logBoxTitleText.setCharacterSize(11);
    logBoxTitleText.setFillColor(sf::Color::Black);
    logBoxTitleText.setStyle(sf::Text::Bold);

    logBoxBg.setSize(sf::Vector2f(360.f, 150.f));
    logBoxBg.setFillColor(sf::Color(0, 0, 0));
    logBoxBg.setOutlineThickness(1.5f);
    logBoxBg.setOutlineColor(sf::Color(128, 128, 128));
}

void AntivirusApp::createCheckbox(const std::string& labelText, const std::string& id, float x, float y) {
    ShieldCheckbox item;
    item.shieldId = id;
    item.isChecked = false;

    item.box.setSize(sf::Vector2f(16.f, 16.f));
    item.box.setFillColor(sf::Color::White);
    item.box.setOutlineThickness(1.5f);
    item.box.setOutlineColor(sf::Color::Black);

    item.checkMark.setSize(sf::Vector2f(10.f, 10.f));
    item.checkMark.setFillColor(sf::Color(0, 0, 128));

    item.label.setFont(font);
    item.label.setString(labelText);
    item.label.setCharacterSize(11);
    item.label.setFillColor(sf::Color::Black);

    item.box.setPosition(x, y);
    item.label.setPosition(x + 25.f, y);

    checkboxes.push_back(item);
}

float AntivirusApp::getProtectionLevel() const {
    if (!isMasterShieldActive) return 0.f;
    int checked = 0;
    for (const auto& cb : checkboxes) {
        if (cb.isChecked) checked++;
    }
    return static_cast<float>(checked) / 3.0f;
}

void AntivirusApp::update() {
    VirtualWindow::update();
    if (!isOpen) return;

    if (isToggling) {
        float dt = toggleClock.restart().asSeconds();
        if (dt > 0.1f) dt = 0.016f;
        
        toggleDelayTimer -= dt;
        if (toggleDelayTimer <= 0.f) {
            isMasterShieldActive = targetToggleState;
            isToggling = false;
            std::cout << "[Antivirus] Shield master active state set to: " << isMasterShieldActive << "\n";
        }
    }
}

void AntivirusApp::draw(sf::RenderWindow& window) {
    if (!isOpen) return;
    VirtualWindow::draw(window);
    if (getIsOpening()) return;

    sf::Vector2f winPos = windowFrame.getPosition();

    // Position ON/OFF Button & labels dynamically
    toggleBtn.setPosition(winPos.x + 290.f, winPos.y + 40.f);
    toggleBtnText.setPosition(winPos.x + 305.f, winPos.y + 45.f);

    statusHeader.setPosition(winPos.x + 20.f, winPos.y + 45.f);
    descText.setPosition(winPos.x + 20.f, winPos.y + 75.f);

    if (isToggling) {
        statusHeader.setString("INITIALIZING SHIELD...");
        statusHeader.setFillColor(sf::Color(128, 64, 0));
        toggleBtn.setFillColor(sf::Color(160, 160, 160));
        toggleBtnText.setString("WAIT...");
    } else if (isMasterShieldActive) {
        statusHeader.setString("AMITY SHIELD ACTIVE GUARD - ON");
        statusHeader.setFillColor(sf::Color(0, 128, 0));
        toggleBtn.setFillColor(sf::Color(220, 100, 100));
        toggleBtnText.setString("TURN OFF");
    } else {
        statusHeader.setString("AMITY SHIELD ACTIVE GUARD - OFF");
        statusHeader.setFillColor(sf::Color(120, 120, 120));
        toggleBtn.setFillColor(sf::Color(100, 200, 100));
        toggleBtnText.setString("TURN ON");
    }

    window.draw(statusHeader);
    window.draw(descText);
    window.draw(toggleBtn);
    window.draw(toggleBtnText);

    for (auto& cb : checkboxes) {
        float ry = cb.box.getPosition().y;
        float rx = cb.box.getPosition().x;

        sf::Vector2f absoluteBoxPos = winPos + sf::Vector2f(rx, ry);
        cb.box.setPosition(absoluteBoxPos);
        cb.label.setPosition(absoluteBoxPos.x + 25.f, absoluteBoxPos.y + 1.f);
        cb.checkMark.setPosition(absoluteBoxPos.x + 3.f, absoluteBoxPos.y + 3.f);

        // Gray out checkbox UI styles if master shield is off
        if (isMasterShieldActive && !isToggling) {
            cb.box.setFillColor(sf::Color::White);
            cb.label.setFillColor(sf::Color::Black);
            cb.checkMark.setFillColor(sf::Color(0, 0, 128));
        } else {
            cb.box.setFillColor(sf::Color(210, 210, 210));
            cb.label.setFillColor(sf::Color(128, 128, 128));
            cb.checkMark.setFillColor(sf::Color(128, 128, 128));
        }

        window.draw(cb.box);
        if (cb.isChecked) {
            window.draw(cb.checkMark);
        }
        window.draw(cb.label);

        cb.box.setPosition(rx, ry);
    }

    logBoxTitleText.setPosition(winPos.x + 20.f, winPos.y + 205.f);
    logBoxBg.setPosition(winPos.x + 20.f, winPos.y + 225.f);
    window.draw(logBoxTitleText);
    window.draw(logBoxBg);

    for (size_t i = 0; i < blockedThreatLogs.size(); ++i) {
        sf::Text txt;
        txt.setFont(font);
        txt.setString(blockedThreatLogs[i]);
        txt.setCharacterSize(11);
        txt.setFillColor(sf::Color(0, 255, 0));
        txt.setPosition(winPos.x + 30.f, winPos.y + 235.f + i * 18.f);
        window.draw(txt);
    }
}

void AntivirusApp::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (!isOpen) return;

    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF = window.mapPixelToCoords(pixelPos);
    sf::Vector2f winPos = windowFrame.getPosition();

    // Let title bar drag work even during toggles
    VirtualWindow::handleEvent(event, window);

    if (isToggling) return;

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::FloatRect toggleBounds(winPos.x + 290.f, winPos.y + 40.f, 90.f, 25.f);
        if (toggleBounds.contains(mousePosF)) {
            isToggling = true;
            targetToggleState = !isMasterShieldActive;
            
            float delay = (ramLagMultiplier - 2.5f) * 0.22f;
            if (delay < 0.5f) delay = 0.5f;
            if (delay > 2.5f) delay = 2.5f;
            toggleDelayTimer = delay;
            toggleClock.restart();
            
            std::cout << "[Antivirus] Toggling shield guard... (thinking time: " << delay << "s)\n";
            return;
        }

        if (isMasterShieldActive) {
            for (auto& cb : checkboxes) {
                float rx = cb.box.getPosition().x;
                float ry = cb.box.getPosition().y;
                sf::FloatRect absoluteBounds(winPos.x + rx, winPos.y + ry, cb.box.getSize().x, cb.box.getSize().y);

                if (absoluteBounds.contains(mousePosF)) {
                    bool targetState = !cb.isChecked;
                    for (auto& other : checkboxes) {
                        other.isChecked = false;
                    }
                    cb.isChecked = targetState;
                    std::cout << "[Antivirus] Toggled " << cb.shieldId << " to: " << cb.isChecked << "\n";
                    break;
                }
            }
        }
    }
}

void AntivirusApp::logBlockedThreat(const std::string& threatName) {
    if (blockedThreatLogs.size() >= 7) {
        blockedThreatLogs.erase(blockedThreatLogs.begin());
    }
    blockedThreatLogs.push_back("[BLOCKED] " + threatName);
}

bool AntivirusApp::isFileShieldActive() const {
    if (!isMasterShieldActive) return false;
    for (const auto& cb : checkboxes) {
        if (cb.shieldId == "file_shield" && cb.isChecked) return true;
    }
    return false;
}

bool AntivirusApp::isActiveMonitorActive() const {
    if (!isMasterShieldActive) return false;
    for (const auto& cb : checkboxes) {
        if (cb.shieldId == "active_shield" && cb.isChecked) return true;
    }
    return false;
}

bool AntivirusApp::isMemoryFirewallActive() const {
    if (!isMasterShieldActive) return false;
    for (const auto& cb : checkboxes) {
        if (cb.shieldId == "mem_shield" && cb.isChecked) return true;
    }
    return false;
}
