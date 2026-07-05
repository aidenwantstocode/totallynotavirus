#ifndef WINDOW_MANAGER_HPP
#define WINDOW_MANAGER_HPP

#include "virtualWindow.hpp"
#include "desktop.hpp"
#include <vector>

class WindowManager {
private:
    std::vector<VirtualWindow*> windowStack;

public:
    void addWindow(VirtualWindow* window);
    void bringToFront(VirtualWindow* windowToFocus, sf::Vector2f iconPos = sf::Vector2f(-1.f, -1.f));
    void processWindowEvents(const sf::Event& event, const sf::RenderWindow& window, bool desktopIconClicked, VirtualWindow* forcedFocusWindow = nullptr, sf::Vector2f iconPos = sf::Vector2f(-1.f, -1.f));
    void drawWindows(sf::RenderWindow& window);
};

#endif
