#ifndef DRIVE_RECOVERY_APP_HPP
#define DRIVE_RECOVERY_APP_HPP

#include "virtualWindow.hpp"
#include <vector>
#include <string>

class DriveRecoveryApp : public VirtualWindow {
private:
    sf::Font font;
    sf::RectangleShape startBtn;
    sf::Text startBtnText;
    
    sf::RectangleShape progressBarBg;
    sf::RectangleShape progressBar;
    sf::Text progressPercentText;
    
    sf::RectangleShape logBoxBg;
    sf::Text logText;
    std::vector<std::string> logLines;
    
    bool isRunning;
    float progress;
    sf::Clock progressClock;
    
    bool bsodTriggered;
    bool corruptedMode;
    bool waveActive;
    int currentWave;
    float nextCheckpoint;

    int currentSector = 1;
    bool sectorLocked = false;
    
    void addLog(const std::string& message);
    void updateLogText();

public:
    DriveRecoveryApp();
    void update() override;
    void draw(sf::RenderWindow& window) override;
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;

    void startDiagnostics();
    bool isBsodTriggered() const { return bsodTriggered; }
    void clearBsodTriggered() { bsodTriggered = false; }
    
    void enterCorruptedMode();
    float getProgress() const { return progress; }
    void setProgress(float val) { progress = val; }
    
    bool isWaveActive() const { return waveActive; }
    void setWaveActive(bool active) { waveActive = active; }
    int getCurrentWave() const { return currentWave; }
    void triggerNextWave(int waveNum, float checkpoint);
    
    bool getIsRunning() const { return isRunning; }

    int getCurrentSector() const { return currentSector; }
    bool isSectorLocked() const { return sectorLocked; }
    void setSectorLocked(bool locked) { sectorLocked = locked; }
    void unlockSector(int sectorNum);
};

#endif
