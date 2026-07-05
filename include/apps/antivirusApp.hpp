#ifndef ANTIVIRUS_APP_HPP
#define ANTIVIRUS_APP_HPP

#include "virtualWindow.hpp"
#include <vector>
#include <string>

struct ShieldCheckbox {
    sf::RectangleShape box;
    sf::RectangleShape checkMark;
    sf::Text label;
    bool isChecked;
    std::string shieldId;
};

class AntivirusApp : public VirtualWindow {
private:
    sf::Font font;
    sf::Text statusHeader;
    sf::Text descText;
    std::vector<ShieldCheckbox> checkboxes;

    void createCheckbox(const std::string& labelText, const std::string& id, float x, float y);

public:
    AntivirusApp();
    void update() override;
    void draw(sf::RenderWindow& window) override;
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;

    float getProtectionLevel() const;
    bool isFileShieldActive() const;
    bool isActiveMonitorActive() const;
    bool isMemoryFirewallActive() const;
};

#endif
