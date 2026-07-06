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
#include "apps/antivirusApp.hpp"
#include "apps/defragApp.hpp"
#include "engine/windowManager.hpp"

enum class GameState {
    BootSequence,
    NormalOS,
    HardwarePrompt,
    HardwareCancelSequence,
    MountingDiagnostic,
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
    AntivirusApp antivirusApp;
    DefragApp defragApp;
    WindowManager windowManager;
    const unsigned int SCREEN_WIDTH = 1024;
    const unsigned int SCREEN_HEIGHT = 768;
    bool isFullscreen = false;

    // BSOD UI
    sf::RectangleShape bsodBg;
    sf::Text bsodText;
    sf::Clock bsodClock;
    float bsodDuration = 8.0f;

    float systemDelayMultiplier;
    bool isDriveRecoveryCorrupted = false;
    bool isNextBootCorrupted = false;
    float cpuUtil;
    float ramUtil;
    float cpuTemp;
    float overheatTimer;
    bool isOverheating;
    bool isInputFrozen = false;
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
    float systemLogsSpawnTimer = -1.f;
    sf::RectangleShape usbPopupFrame;
    sf::RectangleShape usbPopupTitleBar;

    sf::Clock diagnosticClock;
    float diagnosticProgress = 0.f;
    sf::RectangleShape diagnosticProgressBar;
    sf::RectangleShape diagnosticProgressBarBg;
    sf::Text diagnosticProgressText;
    sf::Text usbPopupTitleText;
    sf::Text usbPopupBodyText;
    sf::RectangleShape usbAcceptButton;
    sf::Text usbAcceptText;
    sf::RectangleShape usbCancelButton;
    sf::Text usbCancelText;

    // Wave Management
    int currentWave = 0;
    float waveTimer = 0.f;
    float popupSpawnTimer = 0.f;
    float fileSpawnTimer = 0.f;

    struct PopupAlert {
        sf::RectangleShape frame;
        sf::RectangleShape titleBar;
        sf::RectangleShape closeButton;
        sf::Text titleText;
        sf::Text bodyText;
        sf::Text closeText;
        sf::Vector2f velocity;
        bool isClosed = false;

        int type = 0; // 0 = standard, 1 = timebomb, 2 = leaker
        float timer = 8.f;
        int clicksNeeded = 5;
        sf::RectangleShape defuseButton;
        sf::Text defuseText;
        float leakTimer = 1.5f;

        PopupAlert(sf::Font& font, float x, float y, sf::Vector2f vel, int t) {
            type = t;
            velocity = vel;
            frame.setSize(sf::Vector2f(220.f, 130.f));
            frame.setFillColor(sf::Color(192, 192, 192));
            frame.setOutlineThickness(1.5f);
            frame.setOutlineColor(sf::Color::White);

            titleBar.setSize(sf::Vector2f(220.f, 20.f));
            titleBar.setFillColor(t == 1 ? sf::Color(200, 0, 0) : (t == 2 ? sf::Color(128, 0, 128) : sf::Color(0, 0, 128)));

            closeButton.setSize(sf::Vector2f(16.f, 16.f));
            closeButton.setFillColor(sf::Color(192, 192, 192));
            closeButton.setOutlineThickness(1.f);
            closeButton.setOutlineColor(sf::Color::Black);

            titleText.setFont(font);
            titleText.setString(t == 1 ? "TIME BOMB DETECTED!" : (t == 2 ? "VXD MEMORY LEAK!" : "ALERT"));
            titleText.setCharacterSize(10);
            titleText.setFillColor(sf::Color::White);
            titleText.setStyle(sf::Text::Bold);

            bodyText.setFont(font);
            if (t == 1) {
                bodyText.setString("System payload executing...\nBOOM in 8.0 seconds!\nMash DEFUSE!");
            } else if (t == 2) {
                bodyText.setString("Conventional heap leakage active.\nLeaking cluster data to desktop.\nClick [X] to isolate.");
            } else {
                bodyText.setString("WARNING!\nConventional memory overflow.\nClose applications.");
            }
            bodyText.setCharacterSize(10);
            bodyText.setFillColor(sf::Color::Black);

            closeText.setFont(font);
            closeText.setString("X");
            closeText.setCharacterSize(10);
            closeText.setFillColor(sf::Color::Black);
            closeText.setStyle(sf::Text::Bold);

            if (t == 1) {
                defuseButton.setSize(sf::Vector2f(80.f, 22.f));
                defuseButton.setFillColor(sf::Color(220, 220, 220));
                defuseButton.setOutlineThickness(1.f);
                defuseButton.setOutlineColor(sf::Color::Black);

                defuseText.setFont(font);
                defuseText.setString("DEFUSE (5)");
                defuseText.setCharacterSize(10);
                defuseText.setFillColor(sf::Color::Black);
                defuseText.setStyle(sf::Text::Bold);
            }

            setPosition(x, y);
        }

        void setPosition(float x, float y) {
            frame.setPosition(x, y);
            titleBar.setPosition(x, y);
            closeButton.setPosition(x + frame.getSize().x - 20.f, y + 2.f);
            titleText.setPosition(x + 5.f, y + 3.f);
            bodyText.setPosition(x + 10.f, y + 30.f);
            closeText.setPosition(x + frame.getSize().x - 15.f, y + 4.f);
            if (type == 1) {
                defuseButton.setPosition(x + 70.f, y + 95.f);
                defuseText.setPosition(x + 85.f, y + 99.f);
            }
        }
    };

    std::vector<PopupAlert> activePopups;
    void spawnPopup();
    void updatePopups(float dt);
    void drawPopups();

    void initWindow();
    void toggleFullscreen();
    void updateWindowView(unsigned int windowWidth, unsigned int windowHeight);
    void closeAllWindows();

public:
    Game();
    void run();

private:
    void processEvents();
    void update();
    void render();
};

#endif