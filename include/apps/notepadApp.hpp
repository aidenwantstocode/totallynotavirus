#ifndef NOTEPAD_APP_HPP
#define NOTEPAD_APP_HPP
#include "virtualWindow.hpp"
#include <string>

class NotepadApp : public VirtualWindow {
private:
    sf::Font font;
    sf::Text contentText;

public:
    NotepadApp();
    void openFile(const std::string& filename, const std::string& content);
    void update() override;
    void draw(sf::RenderWindow& window) override;
};

#endif
