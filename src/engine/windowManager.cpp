#include "engine/windowManager.hpp"
#include <algorithm>

void WindowManager::addWindow(VirtualWindow* window) {
    windowStack.push_back(window);
}

void WindowManager::bringToFront(VirtualWindow* windowToFocus, sf::Vector2f iconPos) {
    if (!windowToFocus) return;

    if (!windowToFocus->getIsOpen()) {
        sf::Vector2f startPos = iconPos;
        if (startPos.x < 0.f && startPos.y < 0.f) {
            startPos = windowToFocus->getPosition() + windowToFocus->getSize() / 2.f;
        }
        windowToFocus->triggerOpenAnimation(startPos);
    }

    auto it = std::find(windowStack.begin(), windowStack.end(), windowToFocus);
    if (it != windowStack.end()) {
        windowStack.erase(it);
        windowStack.push_back(windowToFocus);
    }
    for (VirtualWindow* w : windowStack) {
        if (w != windowToFocus) {
            w->setHasFocus(false);
        }
    }
    windowToFocus->setIsOpen(true);
    windowToFocus->setHasFocus(true);
}

void WindowManager::processWindowEvents(const sf::Event& event, const sf::RenderWindow& window, bool desktopIconClicked, VirtualWindow* forcedFocusWindow, sf::Vector2f iconPos) {
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

    VirtualWindow* clickedWindow = nullptr;
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && !desktopIconClicked) {
        for (auto it = windowStack.rbegin(); it != windowStack.rend(); ++it) {
            if ((*it)->getIsOpen() && (*it)->containsPoint(mousePos)) {
                clickedWindow = *it;
                break;
            }
        }
        if (clickedWindow) {
            bringToFront(clickedWindow);
        } else {
            for (VirtualWindow* w : windowStack) {
                w->setHasFocus(false);
            }
        }
    }

    if (forcedFocusWindow) {
        bringToFront(forcedFocusWindow, iconPos);
    }

    for (auto it = windowStack.rbegin(); it != windowStack.rend(); ++it) {
        VirtualWindow* w = *it;
        if (!w->getIsOpen()) continue;

        if (event.type == sf::Event::MouseButtonPressed) {
            if (desktopIconClicked) continue;
            if (w == clickedWindow) {
                w->handleEvent(event, window);
            }
        } else {
            if (w->getHasFocus() || event.type == sf::Event::MouseMoved || event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::TextEntered || event.type == sf::Event::KeyPressed) {
                w->handleEvent(event, window);
            }
        }
    }
}

void WindowManager::drawWindows(sf::RenderWindow& window) {
    for (VirtualWindow* w : windowStack) {
        if (w->getIsOpen()) {
            w->draw(window);
        }
    }
}
