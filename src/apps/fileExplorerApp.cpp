#include "apps/fileExplorerApp.hpp"
#include <iostream>

FileExplorerApp::FileExplorerApp() : VirtualWindow("File Explorer", 500, 400) {
    isOpen = false;
    selectedIndex = -1;
    activationRequested = false;
    basementDriveVisible = false;
    windowFrame.setFillColor(sf::Color(240, 240, 240));

    if (!font.loadFromFile("c:/WINDOWS/Fonts/CONSOLA.TTF")) {
        std::cerr << "[ERROR] FileExplorerApp FAILED TO LOAD FONT\n";
    }

    sidebar.setSize(sf::Vector2f(150, windowFrame.getSize().y - 50));
    sidebar.setFillColor(sf::Color(220, 220, 220));
    sidebar.setOutlineThickness(1);
    sidebar.setOutlineColor(sf::Color(180, 180, 180));

    contentPane.setSize(sf::Vector2f(windowFrame.getSize().x - sidebar.getSize().x - 45, windowFrame.getSize().y - 60));
    contentPane.setFillColor(sf::Color(255, 255, 255));
    contentPane.setOutlineThickness(1);
    contentPane.setOutlineColor(sf::Color(200, 200, 200));

    addressBar.setSize(sf::Vector2f(contentPane.getSize().x - 160, 24));
    addressBar.setFillColor(sf::Color(235, 235, 235));
    addressBar.setOutlineThickness(1);
    addressBar.setOutlineColor(sf::Color(190, 190, 190));

    backButton.setSize(sf::Vector2f(65, 24));
    backButton.setFillColor(sf::Color(210, 210, 210));
    backButton.setOutlineThickness(1);
    backButton.setOutlineColor(sf::Color(170, 170, 170));

    deleteButton.setSize(sf::Vector2f(70, 24));
    deleteButton.setFillColor(sf::Color(220, 170, 170));
    deleteButton.setOutlineThickness(1);
    deleteButton.setOutlineColor(sf::Color(170, 120, 120));

    addressText.setFont(font);
    addressText.setCharacterSize(12);
    addressText.setFillColor(sf::Color(30, 30, 30));
    addressText.setStyle(sf::Text::Bold);
    addressText.setString("C:\\");

    backText.setFont(font);
    backText.setCharacterSize(12);
    backText.setFillColor(sf::Color(30, 30, 30));
    backText.setString("Back");

    deleteText.setFont(font);
    deleteText.setCharacterSize(12);
    deleteText.setFillColor(sf::Color(50, 10, 10));
    deleteText.setString("Delete");
    deleteText.setStyle(sf::Text::Bold);

    loadFileSystem();

    // Setup shortcuts
    SidebarShortcut pcShortcut;
    pcShortcut.name = "My Computer";
    pcShortcut.path = "C:\\";
    pcShortcut.isVisible = true;
    sidebarShortcuts.push_back(pcShortcut);

    SidebarShortcut desktopShortcut;
    desktopShortcut.name = "Desktop";
    desktopShortcut.path = "C:\\Desktop";
    desktopShortcut.isVisible = true;
    sidebarShortcuts.push_back(desktopShortcut);

    SidebarShortcut basementShortcut;
    basementShortcut.name = "Basement Drive (D:)";
    basementShortcut.path = "D:\\";
    basementShortcut.isVisible = false;
    sidebarShortcuts.push_back(basementShortcut);

    currentPath = "C:\\";
    refreshVisibleItems();
    selectItem(0);
    updateLayout();
}

void FileExplorerApp::loadFileSystem() {
    fileSystem = {
        {"todo_list.txt", "C:\\todo_list.txt", "txt", false, {}},
        {"system_log.txt", "C:\\system_log.txt", "txt", false, {}},
        {"readme.md", "C:\\readme.md", "txt", false, {}},
        {"Desktop", "C:\\Desktop", "folder", true, {}},
        {"sys", "C:\\sys", "folder", true, {
            {"drivers", "C:\\sys\\drivers", "folder", true, {
                {"recovery_guide.txt", "C:\\sys\\drivers\\recovery_guide.txt", "txt", false, {}}
            }}
        }},
        {"Program Files", "C:\\Program Files", "folder", true, {}},
        {"Amity", "C:\\Amity", "folder", true, {}}
    };

    basementSystem = {
        {"basement_key.txt", "D:\\basement_key.txt", "txt", false, {}},
        {"encrypted_data.bin", "D:\\encrypted_data.bin", "txt", false, {}},
        {"memories.txt", "D:\\memories.txt", "txt", false, {}}
    };
}

static FileEntry* findEntryByPath(std::vector<FileEntry>& entries, const std::string& path) {
    for (auto& entry : entries) {
        if (entry.path == path) {
            return &entry;
        }
        if (entry.isFolder) {
            FileEntry* childResult = findEntryByPath(entry.children, path);
            if (childResult) {
                return childResult;
            }
        }
    }
    return nullptr;
}

void FileExplorerApp::refreshVisibleItems() {
    visibleItems.clear();
    itemBackgrounds.clear();
    itemLabels.clear();

    if (currentPath == "C:\\") {
        visibleItems = fileSystem;
    } else if (currentPath == "D:\\") {
        visibleItems = basementSystem;
    } else {
        FileEntry* entry = findEntryByPath(fileSystem, currentPath);
        if (!entry) {
            entry = findEntryByPath(basementSystem, currentPath);
        }
        if (entry && entry->isFolder) {
            visibleItems = entry->children;
        }
    }

    for (size_t i = 0; i < visibleItems.size(); ++i) {
        const auto& item = visibleItems[i];
        sf::RectangleShape itemBg;
        itemBg.setSize(sf::Vector2f(contentPane.getSize().x - 10.f, 30.f));
        if (item.isFolder) {
            itemBg.setFillColor(sf::Color(245, 222, 135));
            itemBg.setOutlineColor(sf::Color(200, 180, 100));
        } else {
            itemBg.setFillColor(sf::Color(255, 255, 255));
            itemBg.setOutlineColor(sf::Color(200, 200, 200));
        }
        itemBg.setOutlineThickness(1.f);
        itemBackgrounds.push_back(itemBg);

        sf::Text label;
        label.setFont(font);
        label.setString((item.isFolder ? "[Folder] " : "") + item.name);
        label.setCharacterSize(11);
        label.setFillColor(sf::Color(20, 20, 20));
        itemLabels.push_back(label);
    }
}

std::string FileExplorerApp::getSelectedItemType() const {
    if (selectedIndex < 0 || selectedIndex >= static_cast<int>(visibleItems.size())) {
        return "";
    }
    return visibleItems[selectedIndex].type;
}

std::string FileExplorerApp::getSelectedItemPath() const {
    if (selectedIndex < 0 || selectedIndex >= static_cast<int>(visibleItems.size())) {
        return "";
    }
    return visibleItems[selectedIndex].path;
}

std::string FileExplorerApp::getSelectedItemName() const {
    if (selectedIndex < 0 || selectedIndex >= static_cast<int>(visibleItems.size())) {
        return "";
    }
    return visibleItems[selectedIndex].name;
}

void FileExplorerApp::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    VirtualWindow::handleEvent(event, window);
    if (!isOpen) return;

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

        // Sidebar shortcuts
        for (const auto& shortcut : sidebarShortcuts) {
            if (shortcut.isVisible && shortcut.rect.getGlobalBounds().contains(mousePos)) {
                currentPath = shortcut.path;
                refreshVisibleItems();
                if (!visibleItems.empty()) {
                    selectItem(0);
                } else {
                    selectItem(-1);
                }
                updateLayout();
                return;
            }
        }

        // Back button
        if (backButton.getGlobalBounds().contains(mousePos) && currentPath != "C:\\" && currentPath != "D:\\") {
            navigateUp();
            return;
        }

        // Delete button
        if (deleteButton.getGlobalBounds().contains(mousePos)) {
            deleteSelectedFile();
            return;
        }

        // Content pane item clicks
        bool itemClicked = false;
        for (int i = 0; i < static_cast<int>(itemBackgrounds.size()); ++i) {
            if (itemBackgrounds[i].getGlobalBounds().contains(mousePos)) {
                itemClicked = true;
                if (selectedIndex == i && doubleClickTimer.getElapsedTime().asMilliseconds() < 300) {
                    if (visibleItems[i].isFolder) {
                        enterFolder(i);
                    } else {
                        activationRequested = true;
                    }
                    doubleClickTimer.restart();
                } else {
                    selectItem(i);
                    doubleClickTimer.restart();
                }
                break;
            }
        }

        if (!itemClicked && contentPane.getGlobalBounds().contains(mousePos)) {
            selectItem(-1);
        }
    }
}

void FileExplorerApp::selectItem(int index) {
    if (index < 0 || index >= static_cast<int>(visibleItems.size())) {
        selectedIndex = -1;
        titleText.setString("File Explorer");
        addressText.setString(currentPath);
        
        for (int i = 0; i < static_cast<int>(itemBackgrounds.size()); ++i) {
            if (visibleItems[i].isFolder) {
                itemBackgrounds[i].setFillColor(sf::Color(245, 222, 135));
                itemBackgrounds[i].setOutlineColor(sf::Color(200, 180, 100));
            } else if (visibleItems[i].type == "installer") {
                itemBackgrounds[i].setFillColor(sf::Color(220, 200, 240));
                itemBackgrounds[i].setOutlineColor(sf::Color(180, 150, 200));
            } else {
                itemBackgrounds[i].setFillColor(sf::Color(255, 255, 255));
                itemBackgrounds[i].setOutlineColor(sf::Color(200, 200, 200));
            }
        }
        return;
    }

    selectedIndex = index;
    const std::string selectedName = visibleItems[index].name;
    titleText.setString("File Explorer - " + selectedName);
    addressText.setString(currentPath);

    for (int i = 0; i < static_cast<int>(itemBackgrounds.size()); ++i) {
        if (i == selectedIndex) {
            itemBackgrounds[i].setFillColor(sf::Color(180, 200, 240));
            itemBackgrounds[i].setOutlineColor(sf::Color(100, 150, 220));
        } else {
            if (visibleItems[i].isFolder) {
                itemBackgrounds[i].setFillColor(sf::Color(245, 222, 135));
                itemBackgrounds[i].setOutlineColor(sf::Color(200, 180, 100));
            } else if (visibleItems[i].type == "installer") {
                itemBackgrounds[i].setFillColor(sf::Color(220, 200, 240));
                itemBackgrounds[i].setOutlineColor(sf::Color(180, 150, 200));
            } else {
                itemBackgrounds[i].setFillColor(sf::Color(255, 255, 255));
                itemBackgrounds[i].setOutlineColor(sf::Color(200, 200, 200));
            }
        }
    }
}

std::string FileExplorerApp::getParentPath(const std::string& path) const {
    if (path == "C:\\" || path == "D:\\") {
        return path;
    }
    size_t pos = path.find_last_of('\\');
    if (pos == std::string::npos || pos <= 2) {
        if (path.rfind("D:\\", 0) == 0) {
            return "D:\\";
        }
        return "C:\\";
    }
    return path.substr(0, pos);
}

void FileExplorerApp::addFileToDesktop(const std::string& filename, const std::string& type) {
    for (auto& entry : fileSystem) {
        if (entry.path == "C:\\Desktop") {
            entry.children.push_back({filename, "C:\\Desktop\\" + filename, type, false, {}});
            if (currentPath == "C:\\Desktop" || currentPath == "C:\\") {
                refreshVisibleItems();
                updateLayout();
            }
            break;
        }
    }
}

void FileExplorerApp::navigateUp() {
    if (currentPath == "C:\\" || currentPath == "D:\\") {
        return;
    }
    currentPath = getParentPath(currentPath);
    refreshVisibleItems();
    if (!visibleItems.empty()) {
        selectItem(0);
    } else {
        selectItem(-1);
    }
    updateLayout();
}

void FileExplorerApp::enterFolder(int index) {
    if (index < 0 || index >= static_cast<int>(visibleItems.size())) {
        return;
    }
    if (!visibleItems[index].isFolder) {
        return;
    }

    currentPath = visibleItems[index].path;
    refreshVisibleItems();
    if (!visibleItems.empty()) {
        selectItem(0);
    } else {
        selectItem(-1);
    }
    updateLayout();
}

void FileExplorerApp::updateShortcuts() {
    sf::Vector2f basePos = windowFrame.getPosition();
    float currentY = basePos.y + 55.f;
    for (auto& shortcut : sidebarShortcuts) {
        if (shortcut.isVisible) {
            shortcut.rect.setSize(sf::Vector2f(sidebar.getSize().x - 10.f, 25.f));
            shortcut.rect.setPosition(sidebar.getPosition().x + 5.f, currentY);
            if (currentPath == shortcut.path) {
                shortcut.rect.setFillColor(sf::Color(160, 180, 220));
            } else {
                shortcut.rect.setFillColor(sf::Color(200, 200, 200));
            }
            shortcut.rect.setOutlineThickness(1.f);
            shortcut.rect.setOutlineColor(sf::Color(120, 120, 120));

            shortcut.text.setFont(font);
            shortcut.text.setString(shortcut.name);
            shortcut.text.setCharacterSize(10);
            shortcut.text.setFillColor(sf::Color::Black);
            shortcut.text.setPosition(shortcut.rect.getPosition().x + 5.f, shortcut.rect.getPosition().y + 5.f);

            currentY += 30.f;
        }
    }
}

void FileExplorerApp::setBasementDriveVisible(bool visible) {
    if (basementDriveVisible != visible) {
        basementDriveVisible = visible;
        for (auto& shortcut : sidebarShortcuts) {
            if (shortcut.name == "Basement Drive (D:)") {
                shortcut.isVisible = visible;
            }
        }
        updateLayout();
    }
}

void FileExplorerApp::updateLayout() {
    sf::Vector2f windowPos = windowFrame.getPosition();
    sidebar.setPosition(windowPos.x + 15, windowPos.y + 45);
    contentPane.setPosition(sidebar.getPosition().x + sidebar.getSize().x + 15, windowPos.y + 45);
    
    addressBar.setPosition(contentPane.getPosition().x + 5, contentPane.getPosition().y + 5);
    backButton.setPosition(contentPane.getPosition().x + contentPane.getSize().x - 150, contentPane.getPosition().y + 5);
    deleteButton.setPosition(contentPane.getPosition().x + contentPane.getSize().x - 80, contentPane.getPosition().y + 5);
    
    addressText.setPosition(addressBar.getPosition().x + 8, addressBar.getPosition().y + 4);
    backText.setPosition(backButton.getPosition().x + 18, backButton.getPosition().y + 4);
    deleteText.setPosition(deleteButton.getPosition().x + 14, deleteButton.getPosition().y + 4);

    updateShortcuts();

    float itemWidth = 45.f;
    float itemHeight = 45.f;
    float paddingX = 55.f;
    float paddingY = 35.f;
    int itemsPerRow = std::max(1, static_cast<int>((contentPane.getSize().x - paddingX) / (itemWidth + paddingX)));

    for (size_t i = 0; i < itemBackgrounds.size(); ++i) {
        int row = i / itemsPerRow;
        int col = i % itemsPerRow;

        float itemX = contentPane.getPosition().x + paddingX + col * (itemWidth + paddingX);
        float itemY = contentPane.getPosition().y + 40.f + row * (itemHeight + paddingY);
        
        itemBackgrounds[i].setSize(sf::Vector2f(itemWidth, itemWidth));
        itemBackgrounds[i].setPosition(itemX, itemY);
        
        // Dynamically fit text with '...' if too long
        std::string originalName = visibleItems[i].name;
        itemLabels[i].setString(originalName);
        float maxTextWidth = itemWidth + paddingX - 10.f; // Predetermined text box width
        
        if (itemLabels[i].getLocalBounds().width > maxTextWidth) {
            std::string truncated = originalName;
            while (truncated.length() > 0) {
                itemLabels[i].setString(truncated + "...");
                if (itemLabels[i].getLocalBounds().width <= maxTextWidth) {
                    break;
                }
                truncated.pop_back();
            }
        }
        
        // Center text under icon
        sf::FloatRect textBounds = itemLabels[i].getLocalBounds();
        float textX = itemX + (itemWidth - textBounds.width) / 2.f;
        float textY = itemY + itemWidth + 5.f;
        itemLabels[i].setPosition(textX, textY);
    }
}

void FileExplorerApp::update() {
    VirtualWindow::update();
    if (isOpen) {
        updateLayout();
    }
}

void FileExplorerApp::draw(sf::RenderWindow& window) {
    VirtualWindow::draw(window);
    if (isOpen) {
        window.draw(sidebar);
        window.draw(contentPane);
        window.draw(addressBar);
        window.draw(addressText);
        window.draw(backButton);
        window.draw(backText);
        window.draw(deleteButton);
        window.draw(deleteText);

        for (const auto& shortcut : sidebarShortcuts) {
            if (shortcut.isVisible) {
                window.draw(shortcut.rect);
                window.draw(shortcut.text);
            }
        }

        for (size_t i = 0; i < itemBackgrounds.size(); ++i) {
            window.draw(itemBackgrounds[i]);
            window.draw(itemLabels[i]);
        }
    }
}

static void recursiveDeleteLeaks(std::vector<FileEntry>& items) {
    auto it = items.begin();
    while (it != items.end()) {
        if (!it->isFolder) {
            if (it->name.find("leak") != std::string::npos) {
                std::cout << "[VFS] Deleting corrupted file: " << it->path << "\n";
                it = items.erase(it);
                continue;
            }
        } else {
            recursiveDeleteLeaks(it->children);
        }
        ++it;
    }
}

void FileExplorerApp::clearCorruptedFiles() {
    recursiveDeleteLeaks(fileSystem);
    refreshVisibleItems();
    updateLayout();
}

static bool recursiveDelete(std::vector<FileEntry>& items, const std::string& path) {
    auto it = items.begin();
    while (it != items.end()) {
        if (it->path == path) {
            std::cout << "[VFS] Erased file/folder: " << path << "\n";
            items.erase(it);
            return true;
        }
        if (it->isFolder) {
            if (recursiveDelete(it->children, path)) {
                return true;
            }
        }
        ++it;
    }
    return false;
}

void FileExplorerApp::deleteSelectedFile() {
    if (selectedIndex < 0 || selectedIndex >= static_cast<int>(visibleItems.size())) {
        return;
    }
    std::string path = visibleItems[selectedIndex].path;
    std::cout << "[VFS] Deleting selected file: " << path << "\n";
    
    bool deleted = false;
    if (path.rfind("D:\\", 0) == 0) {
        deleted = recursiveDelete(basementSystem, path);
    } else {
        deleted = recursiveDelete(fileSystem, path);
    }

    if (deleted) {
        selectItem(-1);
        refreshVisibleItems();
        updateLayout();
    }
}

bool FileExplorerApp::deleteFileByPath(const std::string& path) {
    bool deleted = false;
    if (path.rfind("D:\\", 0) == 0) {
        deleted = recursiveDelete(basementSystem, path);
    } else {
        deleted = recursiveDelete(fileSystem, path);
    }
    if (deleted) {
        selectItem(-1);
        refreshVisibleItems();
        updateLayout();
    }
    return deleted;
}

static int recursiveCountLeaks(const std::vector<FileEntry>& items) {
    int count = 0;
    for (const auto& item : items) {
        if (!item.isFolder) {
            if (item.name.find("leak") != std::string::npos) {
                count++;
            }
        } else {
            count += recursiveCountLeaks(item.children);
        }
    }
    return count;
}

int FileExplorerApp::countCorruptedFiles() const {
    return recursiveCountLeaks(fileSystem) + recursiveCountLeaks(basementSystem);
}