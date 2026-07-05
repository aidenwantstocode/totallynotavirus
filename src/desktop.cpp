#include "desktop.hpp"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cmath>

Desktop::Desktop() {
    isStartMenuOpen = false;
}

void Desktop::init(unsigned int width, unsigned int height) {
    background.setSize(sf::Vector2f(width, height));
    background.setFillColor(sf::Color(0, 128, 128));

    taskbar.setSize(sf::Vector2f(width, 40));
    taskbar.setPosition(0, height - 40);
    taskbar.setFillColor(sf::Color(192, 192, 192));

    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "[ERROR] Desktop FAILED TO LOAD FONT\n";
    }

    startButton.setSize(sf::Vector2f(80, 30));
    startButton.setPosition(5, height - 35);
    startButton.setFillColor(sf::Color(220, 220, 220));
    startButton.setOutlineThickness(1.5f);
    startButton.setOutlineColor(sf::Color::White); 

    startText.setFont(font);
    startText.setString("Start");
    startText.setCharacterSize(14);
    startText.setFillColor(sf::Color::Black);
    startText.setStyle(sf::Text::Bold);
    startText.setPosition(startButton.getPosition().x + 22, startButton.getPosition().y + 5);

    clockText.setFont(font);
    clockText.setCharacterSize(14);
    clockText.setFillColor(sf::Color::Black);
    clockText.setStyle(sf::Text::Bold);
    clockText.setPosition(width - 85, height - 30);

    metricsText.setFont(font);
    metricsText.setCharacterSize(12);
    metricsText.setFillColor(sf::Color::Black);
    metricsText.setStyle(sf::Text::Bold);
    metricsText.setPosition(width - 340.f, height - 28.f);
    showMetrics = false;

    // Start Menu Box Setup
    float menuWidth = 150.f;
    float menuHeight = 80.f;
    float menuX = 5.f;
    float menuY = taskbar.getPosition().y - menuHeight;

    startMenuBox.setSize(sf::Vector2f(menuWidth, menuHeight));
    startMenuBox.setFillColor(sf::Color(192, 192, 192));
    startMenuBox.setOutlineThickness(2.f);
    startMenuBox.setOutlineColor(sf::Color::Black);
    startMenuBox.setPosition(menuX, menuY);

    // Settings item setup
    settingsItem.setSize(sf::Vector2f(menuWidth - 10.f, 30.f));
    settingsItem.setFillColor(sf::Color(192, 192, 192));
    settingsItem.setPosition(menuX + 5.f, menuY + 5.f);

    settingsText.setFont(font);
    settingsText.setString("Control Panel");
    settingsText.setCharacterSize(12);
    settingsText.setFillColor(sf::Color::Black);
    settingsText.setPosition(menuX + 15.f, menuY + 12.f);

    // Shutdown item setup
    shutdownItem.setSize(sf::Vector2f(menuWidth - 10.f, 30.f));
    shutdownItem.setFillColor(sf::Color(192, 192, 192));
    shutdownItem.setPosition(menuX + 5.f, menuY + 40.f);

    shutdownText.setFont(font);
    shutdownText.setString("Shut Down...");
    shutdownText.setCharacterSize(12);
    shutdownText.setFillColor(sf::Color::Black);
    shutdownText.setPosition(menuX + 15.f, menuY + 47.f);

    updateClock();

    //  DESKTOP ICONS WITH UNIQUE FILE IDS
    createIcon("File Explorer", "file_explorer");
    createIcon("Terminal", "cmd");
}

void Desktop::createIcon(const std::string& title, const std::string& id) {
    int slot = 0;
    while (true) {
        bool occupied = false;
        for (const auto& existing : desktopIcons) {
            if (existing.slotIndex == slot) {
                occupied = true;
                break;
            }
        }
        if (!occupied) break;
        slot++;
    }

    DesktopIcon icon;
    icon.appId = id;
    icon.slotIndex = slot;

    float x = 40.0f;
    float y = 40.0f + slot * 90.0f;

    icon.body.setSize(sf::Vector2f(40, 40));
    icon.body.setPosition(x, y);
    icon.body.setFillColor(sf::Color(220, 220, 100));
    icon.body.setOutlineThickness(1);
    icon.body.setOutlineColor(sf::Color::Black);

    icon.label.setFont(font);
    icon.label.setString(title);
    icon.label.setCharacterSize(12);
    icon.label.setFillColor(sf::Color::White);
    icon.label.setPosition(x - 5, y + 45);

    desktopIcons.push_back(icon);
}

void Desktop::removeIcon(const std::string& id) {
    auto it = desktopIcons.begin();
    while (it != desktopIcons.end()) {
        if (it->appId == id) {
            std::cout << "[Desktop] Removed shortcut for " << id << "\n";
            desktopIcons.erase(it);
            return;
        }
        ++it;
    }
}

std::string Desktop::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF = window.mapPixelToCoords(pixelPos);

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (startButton.getGlobalBounds().contains(mousePosF)) {
            std::cout << "[OS Engine] Start button clicked.\n";
            isStartMenuOpen = !isStartMenuOpen;
            startButton.setFillColor(isStartMenuOpen ? sf::Color(160, 160, 160) : sf::Color(220, 220, 220)); 
            return "";
        }

        if (isStartMenuOpen) {
            if (settingsItem.getGlobalBounds().contains(mousePosF) || settingsText.getGlobalBounds().contains(mousePosF)) {
                std::cout << "[OS Engine] Settings clicked.\n";
                isStartMenuOpen = false;
                startButton.setFillColor(sf::Color(220, 220, 220));
                return "open_control_panel";
            }
            else if (shutdownItem.getGlobalBounds().contains(mousePosF) || shutdownText.getGlobalBounds().contains(mousePosF)) {
                std::cout << "[OS Engine] Shutdown clicked.\n";
                isStartMenuOpen = false;
                startButton.setFillColor(sf::Color(220, 220, 220));
                return "shutdown_system";
            }
            else if (!startMenuBox.getGlobalBounds().contains(mousePosF)) {
                isStartMenuOpen = false;
                startButton.setFillColor(sf::Color(220, 220, 220));
            }
        }

        for (const auto& icon : desktopIcons) {
            if (icon.body.getGlobalBounds().contains(mousePosF)) {
                std::cout << "[OS Engine] Icon " << icon.appId << " opened.\n";
                return icon.appId;
            }
        }
    } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        if (!isStartMenuOpen) {
            startButton.setFillColor(sf::Color(220, 220, 220));
        }
    } else if (event.type == sf::Event::MouseMoved) {
        if (isStartMenuOpen) {
            if (settingsItem.getGlobalBounds().contains(mousePosF) || settingsText.getGlobalBounds().contains(mousePosF)) {
                settingsItem.setFillColor(sf::Color(0, 0, 128));
                settingsText.setFillColor(sf::Color::White);
            } else {
                settingsItem.setFillColor(sf::Color(192, 192, 192));
                settingsText.setFillColor(sf::Color::Black);
            }

            if (shutdownItem.getGlobalBounds().contains(mousePosF) || shutdownText.getGlobalBounds().contains(mousePosF)) {
                shutdownItem.setFillColor(sf::Color(0, 0, 128));
                shutdownText.setFillColor(sf::Color::White);
            } else {
                shutdownItem.setFillColor(sf::Color(192, 192, 192));
                shutdownText.setFillColor(sf::Color::Black);
            }
        }
    }
    return "";
}

void Desktop::updateClock() {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    std::stringstream ss;
    ss << std::setfill('0') 
       << std::setw(2) << localTime->tm_hour << ":" 
       << std::setw(2) << localTime->tm_min << ":" 
       << std::setw(2) << localTime->tm_sec;
    clockText.setString(ss.str());
}

void Desktop::update() {
    updateClock();
}

void Desktop::draw(sf::RenderWindow& window) {
    window.draw(background);
    window.draw(taskbar);
    window.draw(startButton);
    window.draw(startText);
    window.draw(clockText);
    if (showMetrics) {
        window.draw(metricsText);
    }

    for (auto& icon : desktopIcons) {
        window.draw(icon.body);
        window.draw(icon.label);
    }
}

void Desktop::drawStartMenu(sf::RenderWindow& window) {
    if (isStartMenuOpen) {
        window.draw(startMenuBox);
        window.draw(settingsItem);
        window.draw(settingsText);
        window.draw(shutdownItem);
        window.draw(shutdownText);
    }
}

void Desktop::setCorruptedTheme(bool enabled) {
    if (enabled) {
        background.setFillColor(sf::Color(120, 0, 0));
    } else {
        background.setFillColor(sf::Color(0, 128, 128));
    }
}

void Desktop::setMetrics(float cpu, float ram, float temp, bool show) {
    metricsCpu = cpu;
    metricsRam = ram;
    metricsTemp = temp;
    showMetrics = show;

    if (showMetrics) {
        std::stringstream ss;
        ss << "CPU: " << static_cast<int>(metricsCpu) << "% | "
           << "RAM: " << static_cast<int>(metricsRam) << "% | "
           << "TEMP: " << static_cast<int>(metricsTemp) << "C";
        metricsText.setString(ss.str());

        if (metricsTemp >= 98.f || metricsCpu >= 95.f) {
            metricsText.setFillColor(sf::Color(255, 0, 0)); // Red
        } else if (metricsTemp >= 70.f || metricsCpu >= 70.f) {
            metricsText.setFillColor(sf::Color(230, 120, 0)); // Orange
        } else {
            metricsText.setFillColor(sf::Color::Black);
        }
    }
}

sf::Vector2f Desktop::getIconPosition(const std::string& appId) const {
    for (const auto& icon : desktopIcons) {
        if (icon.appId == appId) {
            return icon.body.getPosition();
        }
    }
    return sf::Vector2f(200.f, 200.f);
}