#ifndef INSTALLER_APPS_HPP
#define INSTALLER_APPS_HPP

#include "virtualWindow.hpp"
#include <vector>
#include <string>

struct CheckboxItem {
    sf::RectangleShape box;
    sf::RectangleShape checkMark;
    sf::Text label;
    bool isChecked;
    std::string appId;
};

class SoftwareInstallerApp : public VirtualWindow {
private:
    sf::Font font;
    sf::Text headerText;
    sf::Text warningText;
    std::vector<CheckboxItem> checkboxes;
    sf::RectangleShape finalizeBtn;
    sf::Text finalizeText;
    sf::RectangleShape uninstallBtn;
    sf::Text uninstallText;

    bool isFinalized;
    void createCheckbox(const std::string& labelText, const std::string& id, float x, float y);

public:
    SoftwareInstallerApp();
    
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void update() override;
    void draw(sf::RenderWindow& window) override;
    bool isComponentChecked(const std::string& id) const;
    bool getIsFinalized() const { return isFinalized; }
};

#endif
