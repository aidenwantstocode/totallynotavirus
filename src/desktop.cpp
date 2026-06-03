#include "Desktop.hpp"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

Desktop::Desktop() {}

void Desktop::init(unsigned int width, unsigned int height) {
    //teal bg
    background.setSize(sf::Vector2f(width, height));
    background.setFillColor(sf::Color(0, 128, 128));

    //gray taskbar
    taskbar.setSize(sf::Vector2f(width, 40));
    taskbar.setPosition(0, height - 40);
    taskbar.setFillColor(sf::Color(192, 192, 192));

    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "[ERROR] Gagal memuat font dari C:/Windows/Fonts/arial.ttf\n";
    }

    //start button configs
    startButton.setSize(sf::Vector2f(80, 30));
    startButton.setPosition(5, height - 35);
    startButton.setFillColor(sf::Color(220, 220, 220));
    startButton.setOutlineThickness(1.5f);
    // Memberi efek garis tepi putih di atas-kiri dan abu tua di bawah-kanan (efek tombol 3D jadul)
    startButton.setOutlineColor(sf::Color::White); 

    //text configurations for start button
    startText.setFont(font);
    startText.setString("Start");
    startText.setCharacterSize(14);
    startText.setFillColor(sf::Color::Black);
    startText.setStyle(sf::Text::Bold);
    startText.setPosition(startButton.getPosition().x + 22, startButton.getPosition().y + 5);

    //clock configs
    clockText.setFont(font);
    clockText.setCharacterSize(14);
    clockText.setFillColor(sf::Color::Black);
    clockText.setStyle(sf::Text::Bold);
    clockText.setPosition(width - 85, height - 30);

    updateClock();
}

void Desktop::updateClock() {
    //get current local time
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    std::stringstream ss;
    ss << std::setfill('0') 
       << std::setw(2) << localTime->tm_hour << ":" 
       << std::setw(2) << localTime->tm_min << ":" 
       << std::setw(2) << localTime->tm_sec;

    clockText.setString(ss.str());
}

void Desktop::handleInput(sf::Vector2i mousePos) {
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    if (startButton.getGlobalBounds().contains(mousePosF)) {
        std::cout << "[OS Engine] Tombol Start diklik! Membuka menu...\n";
        
        //change button color when clicked
        startButton.setFillColor(sf::Color(160, 160, 160)); 
    } else {
        startButton.setFillColor(sf::Color(220, 220, 220));
    }
}

void Desktop::update() {
    updateClock(); // Selalu sinkronkan jam setiap frame berjalan
}

//draw desktop elements from background to foreground
void Desktop::draw(sf::RenderWindow& window) {
    window.draw(background);
    window.draw(taskbar);
    window.draw(startButton);
    window.draw(startText);
    window.draw(clockText);
}