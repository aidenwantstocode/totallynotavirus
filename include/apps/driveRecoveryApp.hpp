#ifndef DRIVE_RECOVERY_APP_HPP
#define DRIVE_RECOVERY_APP_HPP

#include "virtualWindow.hpp"

class DriveRecoveryApp : public VirtualWindow {
private:
    sf::Font font;
    sf::Text statusText;

public:
    DriveRecoveryApp();
    void update() override;
    void draw(sf::RenderWindow& window) override;
};

#endif
