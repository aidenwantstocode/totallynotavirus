#include "installerApps.hpp"
#include <iostream>

SoftwareInstallerApp::SoftwareInstallerApp() : VirtualWindow("Amity Package Manager v1.0", 500, 380) {
    isOpen = false;
    isFinalized = false;
    windowFrame.setFillColor(sf::Color(192, 192, 192)); // Abu-abu klasik 90an

    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "[ERROR] SoftwareInstallerApp FAILED TO LOAD FONT\n";
    }

    headerText.setFont(font);
    headerText.setString("Select optional components to protect your system:");
    headerText.setCharacterSize(13);
    headerText.setFillColor(sf::Color::Black);

    //latency function text
    warningText.setFont(font);
    warningText.setString("WARNING: Installing optional modules will allocate shared\n"
                         "conventional memory, causing noticeable system latency (LAG).");
    warningText.setCharacterSize(11);
    warningText.setFillColor(sf::Color(130, 0, 0)); //redcolor for warning
    warningText.setStyle(sf::Text::Bold);

    createCheckbox("Amity Shield Antivirus (Suppress 30% minor glitches)", "antivirus", 30.0f, 80.0f);
    createCheckbox("Marrow PC Health Monitor (Show Integrity on Taskbar)", "health_monitor", 30.0f, 130.0f);
    createCheckbox("Memory Abstractor (Emergency system defragmenter)", "abstractor", 30.0f, 180.0f);

    // Setup Tombol Finalize Install
    finalizeBtn.setSize(sf::Vector2f(130, 30));
    finalizeBtn.setFillColor(sf::Color(220, 220, 220));
    finalizeBtn.setOutlineThickness(1.5f);
    finalizeBtn.setOutlineColor(sf::Color::White);

    finalizeText.setFont(font);
    finalizeText.setString("Finalize Install");
    finalizeText.setCharacterSize(12);
    finalizeText.setFillColor(sf::Color::Black);

    uninstallBtn.setSize(sf::Vector2f(130, 30));
    uninstallBtn.setFillColor(sf::Color(170, 170, 170));
    uninstallBtn.setOutlineThickness(1.5f);
    uninstallBtn.setOutlineColor(sf::Color(100, 100, 100));

    uninstallText.setFont(font);
    uninstallText.setString("Uninstall Apps");
    uninstallText.setCharacterSize(12);
    uninstallText.setFillColor(sf::Color(100, 100, 100));
}

void SoftwareInstallerApp::createCheckbox(const std::string& labelText, const std::string& id, float x, float y) {
    CheckboxItem item;
    item.appId = id;
    item.isChecked = false;

    item.box.setSize(sf::Vector2f(16, 16));
    item.box.setFillColor(sf::Color::White);
    item.box.setOutlineThickness(1.5f);
    item.box.setOutlineColor(sf::Color(50, 50, 50));
    item.checkMark.setSize(sf::Vector2f(10, 10));
    item.checkMark.setFillColor(sf::Color::Transparent); 

    //text lables
    item.label.setFont(font);
    item.label.setString(labelText);
    item.label.setCharacterSize(12);
    item.label.setFillColor(sf::Color::Black);
    //relative start position
    item.box.setPosition(x, y);
    item.checkMark.setPosition(x + 3, y + 3);
    item.label.setPosition(x + 25, y + 1);

    checkboxes.push_back(item);
}

void SoftwareInstallerApp::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    VirtualWindow::handleEvent(event, window);
    if (!isOpen) return;

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

        //checkboxes logic (not final yet)
        if (!isFinalized) {
            for (auto& item : checkboxes) {
                if (item.box.getGlobalBounds().contains(mousePos)) {
                    item.isChecked = !item.isChecked;
                    item.checkMark.setFillColor(item.isChecked ? sf::Color(50, 50, 50) : sf::Color::Transparent);
                    std::cout << "[Installer] Toggle " << item.appId << " to: " << item.isChecked << "\n";
                }
            }

            //finalize button logic
            if (finalizeBtn.getGlobalBounds().contains(mousePos)) {
                isFinalized = true;
                finalizeBtn.setFillColor(sf::Color(140, 140, 140)); // Menggelap karena terkunci
                finalizeText.setFillColor(sf::Color(200, 200, 200));
                
                //buitton visual for uninstall button
                uninstallBtn.setFillColor(sf::Color(220, 220, 220));
                uninstallBtn.setOutlineColor(sf::Color::White);
                uninstallText.setFillColor(sf::Color::Black);
                
                std::cout << "[Installer] Installation finalized successfully!\n";
            }
        } 
        //uninstall button logic
        else if (isFinalized && uninstallBtn.getGlobalBounds().contains(mousePos)) {
            std::cout << "[HORROR TRIGGER] Player tried to uninstall. Access Denied Pop-up should appear!\n";
            //upcoming: connect to error window in Game.cpp
        }
    }
}

void SoftwareInstallerApp::update() {
    VirtualWindow::update();
    if (!isOpen) return;

    sf::Vector2f basePos = windowFrame.getPosition();
    headerText.setPosition(basePos.x + 20, basePos.y + 45);
    warningText.setPosition(basePos.x + 20, basePos.y + 240);
    
    for (auto& item : checkboxes) {
        float localY = (item.appId == "antivirus") ? 80.0f : (item.appId == "health_monitor") ? 130.0f : 180.0f;
        item.box.setPosition(basePos.x + 30, basePos.y + localY);
        item.checkMark.setPosition(basePos.x + 33, basePos.y + localY + 3);
        item.label.setPosition(basePos.x + 55, basePos.y + localY + 1);
    }

    finalizeBtn.setPosition(basePos.x + 90, basePos.y + 310);
    finalizeText.setPosition(basePos.x + 112, basePos.y + 317);

    uninstallBtn.setPosition(basePos.x + 260, basePos.y + 310);
    uninstallText.setPosition(basePos.x + 285, basePos.y + 317);
}

void SoftwareInstallerApp::draw(sf::RenderWindow& window) {
    if (!isOpen) return;
    
    VirtualWindow::draw(window);
    window.draw(headerText);
    window.draw(warningText);
    for (const auto& item : checkboxes) {
        window.draw(item.box);
        window.draw(item.checkMark);
        window.draw(item.label);
    }
    window.draw(finalizeBtn);
    window.draw(finalizeText);
    window.draw(uninstallBtn);
    window.draw(uninstallText);
}

bool SoftwareInstallerApp::isComponentChecked(const std::string& id) const {
    for (const auto& item : checkboxes) {
        if (item.appId == id) return item.isChecked;
    }
    return false;
}