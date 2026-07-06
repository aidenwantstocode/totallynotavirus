#ifndef DEFRAG_APP_HPP
#define DEFRAG_APP_HPP

#include "virtualWindow.hpp"

class DefragApp : public VirtualWindow {
private:
    sf::Font font;
    sf::Text statusHeader;
    sf::Text statusInfo;
    
    sf::RectangleShape defragBtn;
    sf::Text defragBtnText;
    
    sf::RectangleShape progressBarBg;
    sf::RectangleShape progressBar;
    sf::Text progressText;
    
    bool isDefragmenting;
    float progress;
    sf::Clock progressClock;
    
    bool cleanRequested;

public:
    DefragApp();
    void update() override;
    void draw(sf::RenderWindow& window) override;
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;

    bool getIsDefragmenting() const { return isDefragmenting; }
    bool checkAndClearCleanRequest();

    void setLeakCount(int leaks);

private:
    int leakCount = 0;
};

#endif
