#ifndef FILE_EXPLORER_APP_HPP
#define FILE_EXPLORER_APP_HPP
#include "virtualWindow.hpp"
#include <vector>
#include <string>

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
    sf::RectangleShape deleteButton;
    sf::Text deleteText;

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

    float scrollOffsetY = 0.f;
    bool isUnsupportedErrorOpen = false;
    sf::RectangleShape errorBg;
    sf::RectangleShape errorOkButton;
    sf::Text errorTitleText;
    sf::Text errorBodyText;
    sf::Text errorOkText;

    bool isDraggingScrollbar = false;
    float dragScrollStartY = 0.f;
    float dragScrollStartOffset = 0.f;

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
    void addFileToDesktop(const std::string& filename, const std::string& type);
    void addFileToFolder(const std::string& folderPath, const std::string& filename, const std::string& type);
    std::vector<std::string> getDesktopFileNames() const;
    void clearCorruptedFiles();
    void deleteSelectedFile();
    bool deleteFileByPath(const std::string& path);
    int countCorruptedFiles() const;
    const std::vector<FileEntry>& getVisibleItems() const { return visibleItems; }
    bool isModalActive() const override { return isUnsupportedErrorOpen; }
};

#endif
