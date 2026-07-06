#ifndef TERMINAL_APP_HPP
#define TERMINAL_APP_HPP

#include "virtualWindow.hpp"
#include <string>

class FileExplorerApp;

class TerminalApp : public VirtualWindow {
private:
    sf::Font font;
    sf::Text terminalText;
    std::string commandHistory;
    std::string currentInput;
    sf::RectangleShape contentPane;

    float delayMultiplier = 1.0f;
    bool isProcessing = false;
    sf::Clock processingClock;
    float requiredProcessingTime = 0.0f;
    std::string currentProcessingCommand = "";
    bool recoveryComplete = false;
    bool installerDownloaded = false;
    FileExplorerApp* fileExplorer = nullptr;

    bool regsyncSuccess = false;
    bool fat32Success = false;
    bool safeSuccess = false;

    int terminalScrollOffset = 0;
    bool isDraggingScrollbar = false;
    float dragScrollStartY = 0.f;

public:
    TerminalApp();
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void update() override;
    void draw(sf::RenderWindow& window) override;
    void setDelayMultiplier(float multiplier);
    bool isRecoveryComplete() const;
    bool isInstallerDownloaded() const;
    void setFileExplorer(FileExplorerApp* explorer);

    bool checkAndClearRegsync();
    bool checkAndClearFat32();
    bool checkAndClearSafe();
};

#endif
