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
    int uninstallClickCount = 0;
    bool isSystemCorrupted = false;

    bool isErrorOpen = false;
    bool isErrorDragged = false;
    sf::Vector2f errorPopupPosition;
    sf::Vector2f errorDragOffset;
    sf::RectangleShape errorBg;
    sf::RectangleShape errorTitleBarBg;
    sf::RectangleShape errorOkButton;
    sf::RectangleShape errorCloseButton;
    sf::Text errorTitleText;
    sf::Text errorBodyText;
    sf::Text errorOkText;
    sf::Text errorCloseText;
    void initErrorPopup();

public:
    SoftwareInstallerApp();
    bool getIsErrorOpen() const { return isErrorOpen; }
    
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void update() override;
    void draw(sf::RenderWindow& window) override;
    bool isComponentChecked(const std::string& id) const;
    bool getIsFinalized() const { return isFinalized; }
    void setSystemCorrupted(bool status);
};

#endif