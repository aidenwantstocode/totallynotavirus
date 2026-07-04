#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "desktop.hpp"
#include "glitchManager.hpp"
#include "coreApps.hpp"
#include "installerApps.hpp"

enum class GameState {
    BootSequence,
    NormalOS,
    HardwarePrompt,
    HardwareCancelSequence,
    ActiveOS
};

class Game {
private:
    sf::RenderWindow window;
    Desktop desktop;
    GlitchManager glitchManager;
    NotepadApp notepad;
    TerminalApp terminal;
    SoftwareInstallerApp installerWizard;
    FileExplorerApp fileExplorer;
    const unsigned int SCREEN_WIDTH = 1024;
    const unsigned int SCREEN_HEIGHT = 768;

    float systemDelayMultiplier;
    bool isDriveRecoveryCorrupted = false;
    bool hasRecalculatedPerformance = false;
    void recalculateSystemPerformance();

    GameState currentState;
    sf::Clock stateClock;
    
    // Boot sequence UI
    sf::Font systemFont;
    sf::Text biosText;
    sf::RectangleShape bootProgressBar;
    sf::RectangleShape bootProgressBarBackground;
    float bootDuration;
    
    // Hardware prompt UI
    float usbTriggerDelay;
    bool usbPluggedIn;
    sf::RectangleShape usbPopupFrame;
    sf::RectangleShape usbPopupTitleBar;
    sf::Text usbPopupTitleText;
    sf::Text usbPopupBodyText;
    sf::RectangleShape usbAcceptButton;
    sf::Text usbAcceptText;
    sf::RectangleShape usbCancelButton;
    sf::Text usbCancelText;

    void initWindow();

public:
    Game();
    void run();

private:
    void processEvents();
    void update();
    void render();
};

#endif