#include "desktop.hpp"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cmath>

Desktop::Desktop() {}

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

    updateClock();

    //  DESKTOP ICONS WITH UNIQUE FILE IDS
    createIcon("todo_list.txt", "txt_todo", 40.0f, 40.0f);     // ID: txt_todo
    createIcon("Terminal", "cmd", 40.0f, 130.0f);              // ID: cmd
    createIcon("system_log.txt", "txt_log", 40.0f, 220.0f);    // ID: txt_log
    createIcon("File Explorer", "file_explorer", 40.0f, 400.0f);    // ID: file_explorer
}

void Desktop::createIcon(const std::string& title, const std::string& id, float x, float y) {
    DesktopIcon icon;
    icon.appId = id;

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

void Desktop::snapToGrid(DesktopIcon& icon) {
    float gridX = 90.0f;
    float gridY = 90.0f;
    float newX = std::round((icon.body.getPosition().x - 10.0f) / gridX) * gridX + 10.0f;
    float newY = std::round((icon.body.getPosition().y - 10.0f) / gridY) * gridY + 10.0f;
    if (newX < 10.0f) newX = 10.0f;
    if (newY < 10.0f) newY = 10.0f;
    icon.body.setPosition(newX, newY);
    icon.label.setPosition(newX - 5.f, newY + 45.f);
}

std::string Desktop::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF = window.mapPixelToCoords(pixelPos);

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (startButton.getGlobalBounds().contains(mousePosF)) {
            std::cout << "[OS Engine] Start button clicked.\n";
            startButton.setFillColor(sf::Color(160, 160, 160)); 
            return "start_menu";
        }
        for (size_t i = 0; i < desktopIcons.size(); ++i) {
            if (desktopIcons[i].body.getGlobalBounds().contains(mousePosF)) {
                if (lastClickedIconIndex == static_cast<int>(i) && doubleClickTimer.getElapsedTime().asMilliseconds() < 300) {
                    std::cout << "[OS Engine] Icon " << desktopIcons[i].appId << " opened.\n";
                    return desktopIcons[i].appId;
                }
                lastClickedIconIndex = i;
                doubleClickTimer.restart();
                draggedIconIndex = i;
                dragOffset = mousePosF - desktopIcons[i].body.getPosition();
                break;
            }
        }
    } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        startButton.setFillColor(sf::Color(220, 220, 220));
        if (draggedIconIndex != -1) {
            snapToGrid(desktopIcons[draggedIconIndex]);
            draggedIconIndex = -1;
        }
    } else if (event.type == sf::Event::MouseMoved) {
        if (draggedIconIndex != -1) {
            float newX = mousePosF.x - dragOffset.x;
            float newY = mousePosF.y - dragOffset.y;
            desktopIcons[draggedIconIndex].body.setPosition(newX, newY);
            desktopIcons[draggedIconIndex].label.setPosition(newX - 5.f, newY + 45.f);
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

    for (auto& icon : desktopIcons) {
        window.draw(icon.body);
        window.draw(icon.label);
    }
}