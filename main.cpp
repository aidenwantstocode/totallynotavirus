//  TOTALLY NOT A VIRUS - An OS Simulation Horror Game Concept Built in C++ with SFML

#include <SFML/Graphics.hpp>
#include <iostream>

class Game {
private:
    sf::RenderWindow window;
    sf::RectangleShape desktopBackground;
    sf::RectangleShape taskBar;

    const unsigned int SCREEN_WIDTH = 1024;
    const unsigned int SCREEN_HEIGHT = 768;

    void initWindow() {
        window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "totallynotavirus - Booting...", sf::Style::Titlebar | sf::Style::Close);
        window.setFramerateLimit(60); // lock at 60 FPS for smoother performance
    }

    void initDesktop() {
        // Desktop background setup
        desktopBackground.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
        desktopBackground.setFillColor(sf::Color(0, 128, 128)); // teal background

        // Taskbar setup
        taskBar.setSize(sf::Vector2f(SCREEN_WIDTH, 40));
        taskBar.setPosition(0, SCREEN_HEIGHT - 40);
        taskBar.setFillColor(sf::Color(192, 192, 192)); // classic silver grey
    }

public:
    // Constructor
    Game() {
        initWindow();
        initDesktop();
    }

    // MAIN GAME LOOP
    void run() {
        while (window.isOpen()) {
            processEvents();
            update();
            render();
        }
    }

private:
    // upcoming feat: Player input detection
    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            // if player clicks the close button, close the window
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            // upcoming feat: Mouse click detection logic
        }
    }

    // upcoming feat: Game logic updates (Virus spreading, file corruption, system instability)
    void update() {
    }

    // upcoming feat: Object display/render
    void render() {
        window.clear(); //clear past frame

        // upcoming feat: Draw desktop background and other game objects
        window.draw(desktopBackground);
        window.draw(taskBar);

        window.display(); // display to monitor
    }
};

int main() {
    Game virusGame;
    virusGame.run();

    return 0;
}