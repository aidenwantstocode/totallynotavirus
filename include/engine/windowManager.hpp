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
    void bringToFront(VirtualWindow* windowToFocus);
    void processWindowEvents(const sf::Event& event, const sf::RenderWindow& window, bool desktopIconClicked, VirtualWindow* forcedFocusWindow = nullptr);
    void drawWindows(sf::RenderWindow& window);
};

#endif
