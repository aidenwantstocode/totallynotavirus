#ifndef APPS_HPP
#define APPS_HPP

#include "virtualWindow.hpp"
#include <string>
#include <vector>

//notepad app
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

//terminal/cmd virus app
class TerminalApp : public VirtualWindow {
private:
    sf::Font font;
    sf::Text terminalText;
    std::string commandHistory;
    std::string currentInput;

    float delayMultiplier = 1.0f;
    bool isProcessing = false;
    sf::Clock processingClock;
    float requiredProcessingTime = 0.0f;
    bool recoveryComplete = false;

public:
    TerminalApp();
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void update() override;
    void draw(sf::RenderWindow& window) override;
    void setDelayMultiplier(float multiplier);
    bool isRecoveryComplete() const;
};

class DriveRecoveryApp : public VirtualWindow {
private:
    sf::Font font;
    sf::Text statusText;

public:
    DriveRecoveryApp();
    void update() override;
    void draw(sf::RenderWindow& window) override;
};

struct FileEntry {
    std::string name;
    std::string path;
    std::string type;
    bool isFolder;
    std::vector<FileEntry> children;
};

struct SidebarShortcut {
    std::string name;
    std::string path;
    sf::RectangleShape rect;
    sf::Text text;
    bool isVisible;
};

class FileExplorerApp : public VirtualWindow {
private:
    sf::Font font;

    sf::RectangleShape sidebar;
    sf::RectangleShape contentPane;
    sf::RectangleShape addressBar;
    sf::RectangleShape backButton;
    sf::Text addressText;
    sf::Text backText;

    std::vector<sf::RectangleShape> itemBackgrounds;
    std::vector<sf::Text> itemLabels;
    std::vector<FileEntry> visibleItems;
    std::vector<FileEntry> fileSystem;
    std::vector<FileEntry> basementSystem;
    std::vector<SidebarShortcut> sidebarShortcuts;
    bool basementDriveVisible = false;

    std::string currentPath;
    int selectedIndex;
    bool activationRequested;
    sf::Clock doubleClickTimer;

    void updateLayout();
    void selectItem(int index);
    void loadFileSystem();
    void refreshVisibleItems();
    std::string getParentPath(const std::string& path) const;
    void navigateUp();
    void enterFolder(int index);
    void updateShortcuts();

public:
    FileExplorerApp();
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void update() override;
    void draw(sf::RenderWindow& window) override;
    
    std::string getSelectedItemType() const;
    std::string getSelectedItemPath() const;
    std::string getSelectedItemName() const;
    bool isActivationRequested() const { return activationRequested; }
    void clearActivationRequest() { activationRequested = false; }
    void setBasementDriveVisible(bool visible);
};

#endif