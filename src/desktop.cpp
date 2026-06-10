#include "Desktop.hpp"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

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
    createIcon("system_log.txt", "txt_log", 40.0f, 220.0f);    // ID: txt_log (BERKAS BARU!)
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

std::string Desktop::handleInput(sf::Vector2i mousePos) {
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    if (startButton.getGlobalBounds().contains(mousePosF)) {
        std::cout << "[OS Engine] Start button clicked.\n";
        startButton.setFillColor(sf::Color(160, 160, 160)); 
        return "start_menu";
    } else {
        startButton.setFillColor(sf::Color(220, 220, 220));
    }

    for (const auto& icon : desktopIcons) {
        if (icon.body.getGlobalBounds().contains(mousePosF)) {
            std::cout << "[OS Engine] Icon " << icon.appId << " clicked.\n";
            return icon.appId;
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