#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "desktop.hpp"
#include "glitchManager.hpp"
#include "apps/notepadApp.hpp"
#include "apps/terminalApp.hpp"
#include "apps/softwareInstallerApp.hpp"
#include "apps/fileExplorerApp.hpp"
#include "apps/settingsApp.hpp"
#include "apps/driveRecoveryApp.hpp"
#include "engine/windowManager.hpp"

enum class GameState {
    BootSequence,
    NormalOS,
    HardwarePrompt,
    HardwareCancelSequence,
    ActiveOS,
    BSOD,
    CorruptedOS
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
    SettingsApp settingsApp;
    DriveRecoveryApp driveRecovery;
    WindowManager windowManager;
    const unsigned int SCREEN_WIDTH = 1024;
    const unsigned int SCREEN_HEIGHT = 768;
    bool isFullscreen = false;

    // BSOD UI
    sf::RectangleShape bsodBg;
    sf::Text bsodText;
    sf::Clock bsodClock;
    float bsodDuration = 6.0f;

    float systemDelayMultiplier;
    bool isDriveRecoveryCorrupted = false;
    bool isNextBootCorrupted = false;
    float cpuUtil;
    float ramUtil;
    float cpuTemp;
    float overheatTimer;
    bool isOverheating;
    bool hasRecalculatedPerformance = false;
    bool hasSpawnedInstaller = false;
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
    void toggleFullscreen();
    void updateWindowView(unsigned int windowWidth, unsigned int windowHeight);

public:
    Game();
    void run();

private:
    void processEvents();
    void update();
    void render();
};

#endif