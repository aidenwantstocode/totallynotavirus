#include "apps/terminalApp.hpp"
#include "apps/fileExplorerApp.hpp"
#include <iostream>

TerminalApp::TerminalApp() : VirtualWindow("Command Prompt", 500, 350) {
    isOpen = false;
    windowFrame.setFillColor(sf::Color(0, 0, 0));

    if (!font.loadFromFile("c:/WINDOWS/Fonts/CONSOLA.TTF")) {
        std::cerr << "[ERROR] TerminalApp FAILED TO LOAD FONT\n";
    }

    terminalText.setFont(font);
    terminalText.setCharacterSize(14);
    terminalText.setFillColor(sf::Color(200, 200, 200));
    terminalText.setLineSpacing(1.2f);
    
    commandHistory = "Amity Operating System [build 13a94]\n(c) Marrow Computers 1994. All rights reserved.\n\nC:\\> ";
    terminalText.setString(commandHistory);
}

void TerminalApp::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    VirtualWindow::handleEvent(event, window);
    
    if (!hasFocus || !isOpen || isProcessing) return;

    if (event.type == sf::Event::TextEntered) {
        
        if (event.text.unicode == 8) {
            if (!currentInput.empty()) {
                currentInput.pop_back();
            }
        }
        else if (event.text.unicode == 13 || event.text.unicode == 10) {
            if (currentInput == "help") {
                commandHistory += currentInput + "\nAvailable commands:\n  help   - Show documentation\n  status - Check virtual OS integrity\n  clear  - Clear screen buffer\n\nC:\\> ";
            } 
            else if (currentInput == "clear") {
                commandHistory = "Amity Operating System [build 13a94]\n(c) Marrow Computers 1994. All rights reserved.\n\nC:\\> ";
            } 
            else if (currentInput == "status") {
                commandHistory += currentInput + "\n[SYSTEM STATUS]: INTEGRITY SECURE\n[FIREWALL]: SHIELD ACTIVE (100%)\n\nC:\\> ";
            } 
            else if (currentInput == "recover") {
                isProcessing = true;
                currentProcessingCommand = "recover";
                requiredProcessingTime = 3.0f * delayMultiplier;
                processingClock.restart();
                commandHistory += "\n[SYSTEM] Initiating Drive Recovery Tool...";
                commandHistory += "\n[SYSTEM] Please wait, processing clusters...\n";
            }
            else if (currentInput == "pkgget install recovery-wizard") {
                isProcessing = true;
                currentProcessingCommand = "pkgget install recovery-wizard";
                requiredProcessingTime = 2.0f * delayMultiplier;
                processingClock.restart();
                commandHistory += "\n[PKGGET] Connecting to repository...";
                commandHistory += "\n[PKGGET] Downloading amityappsetup.exe...\n";
            }
            else if (currentInput == "regsync") {
                isProcessing = true;
                currentProcessingCommand = "regsync";
                requiredProcessingTime = 2.0f * delayMultiplier;
                processingClock.restart();
                commandHistory += "\n[SYSTEM] Querying registry keys...";
                commandHistory += "\n[SYSTEM] Synchronizing node allocation logs...\n";
            }
            else if (currentInput == "unlock FAT32 4") {
                isProcessing = true;
                currentProcessingCommand = "unlock FAT32 4";
                requiredProcessingTime = 2.0f * delayMultiplier;
                processingClock.restart();
                commandHistory += "\n[SYSTEM] Checking FAT32 checksum logs...";
                commandHistory += "\n[SYSTEM] Unlocking allocation entries...\n";
            }
            else if (currentInput == "unlock safe A1-99X-E9") {
                isProcessing = true;
                currentProcessingCommand = "unlock safe A1-99X-E9";
                requiredProcessingTime = 2.0f * delayMultiplier;
                processingClock.restart();
                commandHistory += "\n[SYSTEM] Authorizing Safe Mode Decryption Key...";
                commandHistory += "\n[SYSTEM] Verifying certificate signatures...\n";
            }
            else if (currentInput.rfind("unlock ", 0) == 0) {
                commandHistory += currentInput + "\n[ERROR] Incorrect parameters or unauthorized unlock command.\n\nC:\\> ";
            }
            else if (currentInput.rfind("rm ", 0) == 0 && currentInput.length() > 3) {
                std::string file = currentInput.substr(3);
                bool deleted = false;
                if (fileExplorer) {
                    deleted = fileExplorer->deleteFileByPath("C:\\" + file);
                    if (!deleted) {
                        deleted = fileExplorer->deleteFileByPath("C:\\Desktop\\" + file);
                    }
                }
                if (deleted) {
                    commandHistory += currentInput + "\n[SYSTEM] File deleted successfully.\n\nC:\\> ";
                } else {
                    commandHistory += currentInput + "\n[ERROR] File not found or access denied.\n\nC:\\> ";
                }
            }
            else if (!currentInput.empty()) {
                commandHistory += currentInput + "\n'" + currentInput + "' is not recognized as an internal or external command.\n\nC:\\> ";
            } 
            else {
                commandHistory += "\nC:\\> ";
            }
            currentInput = "";
        }
        else if (event.text.unicode < 128 && event.text.unicode >= 32) {
            currentInput += static_cast<char>(event.text.unicode);
        }

        terminalText.setString(commandHistory + currentInput);
    }
}

void TerminalApp::update() {
    VirtualWindow::update();
    if (isOpen) {
        terminalText.setPosition(windowFrame.getPosition().x + 12, windowFrame.getPosition().y + 40);
    }
    if (isProcessing) {
        if (processingClock.getElapsedTime().asSeconds() >= requiredProcessingTime) {
            isProcessing = false;
            if (currentProcessingCommand == "recover") {
                recoveryComplete = true;
                commandHistory += "\n[SUCCESS] Sector 0x04F2 recovered successfully!";
                commandHistory += "\nAmityOS Key: [A1-99X-E9]\n\nC:\\> ";
            } else if (currentProcessingCommand == "pkgget install recovery-wizard") {
                installerDownloaded = true;
                commandHistory += "\n[SUCCESS] Package 'recovery-wizard' installed.";
                commandHistory += "\nShortcut placed on Desktop.\n\nC:\\> ";
            } else if (currentProcessingCommand == "regsync") {
                regsyncSuccess = true;
                commandHistory += "\n[SUCCESS] Registry nodes synchronized successfully!\n\nC:\\> ";
            } else if (currentProcessingCommand == "unlock FAT32 4") {
                fat32Success = true;
                commandHistory += "\n[SUCCESS] File Allocation Table FAT32 sectors unlocked!\n\nC:\\> ";
            } else if (currentProcessingCommand == "unlock safe A1-99X-E9") {
                safeSuccess = true;
                commandHistory += "\n[SUCCESS] Safe mode security block bypassed.\n\nC:\\> ";
            }
            terminalText.setString(commandHistory + currentInput);
            currentProcessingCommand = "";
        }
    }
}

static std::string wrapText(const std::string& str, size_t lineLimit) {
    std::string result = "";
    std::string currentLine = "";
    for (char c : str) {
        if (c == '\n') {
            result += currentLine + "\n";
            currentLine = "";
        } else {
            currentLine += c;
            if (currentLine.length() >= lineLimit) {
                result += currentLine + "\n";
                currentLine = "";
            }
        }
    }
    result += currentLine;
    return result;
}

void TerminalApp::draw(sf::RenderWindow& window) {
    VirtualWindow::draw(window);
    if (getIsOpening()) return;
    if (isOpen) {
        terminalText.setString(wrapText(commandHistory + currentInput, 45));
        window.draw(terminalText);
    }
}

void TerminalApp::setDelayMultiplier(float multiplier) {
    delayMultiplier = multiplier;
}

bool TerminalApp::isRecoveryComplete() const {
    return recoveryComplete;
}

bool TerminalApp::isInstallerDownloaded() const {
    return installerDownloaded;
}

void TerminalApp::setFileExplorer(FileExplorerApp* explorer) {
    fileExplorer = explorer;
}

bool TerminalApp::checkAndClearRegsync() {
    bool res = regsyncSuccess;
    regsyncSuccess = false;
    return res;
}

bool TerminalApp::checkAndClearFat32() {
    bool res = fat32Success;
    fat32Success = false;
    return res;
}

bool TerminalApp::checkAndClearSafe() {
    bool res = safeSuccess;
    safeSuccess = false;
    return res;
}
