#include "apps/antivirusApp.hpp"
#include <iostream>

AntivirusApp::AntivirusApp() : VirtualWindow("Amity Shield", 320, 260) {
    isOpen = false;
    windowFrame.setFillColor(sf::Color(192, 192, 192));

    if (!font.loadFromFile("c:/WINDOWS/Fonts/CONSOLA.TTF")) {
        std::cerr << "[ERROR] AntivirusApp FAILED TO LOAD FONT\n";
    }

    statusHeader.setFont(font);
    statusHeader.setString("AMITY SHIELD ACTIVE GUARD");
    statusHeader.setCharacterSize(13);
    statusHeader.setFillColor(sf::Color(0, 0, 128));
    statusHeader.setStyle(sf::Text::Bold);

    descText.setFont(font);
    descText.setString("Active shields suppress minor glitches,\nbut consume base conventional memory.");
    descText.setCharacterSize(10);
    descText.setFillColor(sf::Color::Black);

    createCheckbox("Filesystem Guard (popups)", "file_shield", 30.f, 100.f);
    createCheckbox("Active Monitor (scripts)", "active_shield", 30.f, 140.f);
    createCheckbox("Memory Firewall (corruption)", "mem_shield", 30.f, 180.f);
}

void AntivirusApp::createCheckbox(const std::string& labelText, const std::string& id, float x, float y) {
    ShieldCheckbox item;
    item.shieldId = id;
    item.isChecked = true;

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

    // Positions will be offset in draw/handleEvent dynamically
    item.box.setPosition(x, y);
    item.label.setPosition(x + 25.f, y);

    checkboxes.push_back(item);
}

float AntivirusApp::getProtectionLevel() const {
    int checked = 0;
    for (const auto& cb : checkboxes) {
        if (cb.isChecked) checked++;
    }
    return static_cast<float>(checked) / 3.0f;
}

void AntivirusApp::update() {
    // Standard update
}

void AntivirusApp::draw(sf::RenderWindow& window) {
    if (!isOpen) return;
    VirtualWindow::draw(window);

    sf::Vector2f winPos = windowFrame.getPosition();

    statusHeader.setPosition(winPos.x + 20.f, winPos.y + 40.f);
    descText.setPosition(winPos.x + 20.f, winPos.y + 60.f);

    window.draw(statusHeader);
    window.draw(descText);

    for (auto& cb : checkboxes) {
        // Compute current Y relative position
        float ry = cb.box.getPosition().y;
        float rx = cb.box.getPosition().x;

        sf::Vector2f absoluteBoxPos = winPos + sf::Vector2f(rx, ry);
        cb.box.setPosition(absoluteBoxPos);
        cb.label.setPosition(absoluteBoxPos.x + 25.f, absoluteBoxPos.y + 1.f);
        cb.checkMark.setPosition(absoluteBoxPos.x + 3.f, absoluteBoxPos.y + 3.f);

        window.draw(cb.box);
        if (cb.isChecked) {
            window.draw(cb.checkMark);
        }
        window.draw(cb.label);

        // Reset positions to relative coordinate layout values
        cb.box.setPosition(rx, ry);
    }
}

void AntivirusApp::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    VirtualWindow::handleEvent(event, window);

    if (!isOpen) return;

    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF = window.mapPixelToCoords(pixelPos);

    sf::Vector2f winPos = windowFrame.getPosition();

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        for (auto& cb : checkboxes) {
            float rx = cb.box.getPosition().x;
            float ry = cb.box.getPosition().y;
            sf::FloatRect absoluteBounds(winPos.x + rx, winPos.y + ry, cb.box.getSize().x, cb.box.getSize().y);

            if (absoluteBounds.contains(mousePosF)) {
                cb.isChecked = !cb.isChecked;
                std::cout << "[Antivirus] Toggled " << cb.shieldId << " to: " << cb.isChecked << "\n";
                break;
            }
        }
    }
}
