#include <iostream>
#include "Apps.hpp"

//NOTEPAD APP

NotepadApp::NotepadApp() : VirtualWindow("Notepad - todo_list.txt", 450, 350) {
    windowFrame.setFillColor(sf::Color(255, 255, 255)); 

    if (!font.loadFromFile("c:/WINDOWS/Fonts/CONSOLA.TTF")) {
        std::cerr << "[ERROR] NotepadApp FAILED TO LOAD FONT\n";
    }

    //notepad text properties
    contentText.setFont(font);
    contentText.setCharacterSize(13);
    contentText.setFillColor(sf::Color(50, 50, 50)); // Warna abu-abu gelap agar nyaman dibaca
    contentText.setLineSpacing(1.2f);

    std::string diaryText = 
        "lorem ipsum dolor sit amet, consectetur adipiscing elit. ";
        
    contentText.setString(diaryText);
}

void NotepadApp::update() {
    VirtualWindow::update(); 
    if (isOpen) {
        contentText.setPosition(windowFrame.getPosition().x + 15, windowFrame.getPosition().y + 45);
    }
}

void NotepadApp::draw(sf::RenderWindow& window) {
    VirtualWindow::draw(window); 

    if (isOpen) {
        window.draw(contentText);
    }
}

//TERMINAL APP

TerminalApp::TerminalApp() : VirtualWindow("Command Prompt", 520, 380) {
    isOpen = false; // Tersembunyi saat booting awal
    windowFrame.setFillColor(sf::Color::Black); // Layar hitam khas CMD
    titleBar.setFillColor(sf::Color(45, 45, 45)); // Title bar abu-abu gelap

    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "[ERROR] TerminalApp failed to load font.\n";
    }

    terminalText.setFont(font);
    terminalText.setCharacterSize(13);
    terminalText.setFillColor(sf::Color(0, 255, 0)); // Warna Hijau Matrix / Hacker
    terminalText.setLineSpacing(1.1f);

    commandHistory = "totallynotavirus OS [Version 1.0.2026]\n(c) Microsoft Corporation. All rights reserved.\n\nC:\\> ";
    currentInput = "";
    terminalText.setString(commandHistory + currentInput);
}

void TerminalApp::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    // 1. Tetap jalankan fungsi drag-and-drop bawaan induk
    VirtualWindow::handleEvent(event, window);

    if (!isOpen) return;

    // 2. Logika Menangkap Input Keyboard
    if (event.type == sf::Event::TextEntered) {
        
        // Kasus A: Tombol Backspace (Unicode 8) -> Hapus satu karakter terakhir
        if (event.text.unicode == 8) {
            if (!currentInput.empty()) {
                currentInput.pop_back();
            }
        }
        // Kasus B: Tombol Enter (Unicode 13 atau 10) -> Proses perintah
        else if (event.text.unicode == 13 || event.text.unicode == 10) {
            
            if (currentInput == "help") {
                commandHistory += currentInput + "\nAvailable commands:\n  help   - Show documentation\n  status - Check virtual OS integrity\n  clear  - Clear screen buffer\n\nC:\\> ";
            } 
            else if (currentInput == "clear") {
                commandHistory = "C:\\> ";
            } 
            else if (currentInput == "status") {
                commandHistory += currentInput + "\n[SYSTEM STATUS]: INTEGRITY SECURE\n[FIREWALL]: SHIELD ACTIVE (100%)\n\nC:\\> ";
            } 
            else if (!currentInput.empty()) {
                // Perintah tidak dikenal
                commandHistory += currentInput + "\n'" + currentInput + "' is not recognized as an internal or external command.\n\nC:\\> ";
            } 
            else {
                // Enter kosong
                commandHistory += "\nC:\\> ";
            }
            
            currentInput = ""; // Kosongkan baris ketikan baru
        }
        // Kasus C: Karakter Standar ASCII (Karakter printable antara spasi hingga tilde)
        else if (event.text.unicode < 128 && event.text.unicode >= 32) {
            currentInput += static_cast<char>(event.text.unicode);
        }

        // Update string visual terminal
        terminalText.setString(commandHistory + currentInput);
    }
}

void TerminalApp::update() {
    VirtualWindow::update();
    if (isOpen) {
        // Sinkronisasi posisi teks mengikuti koordinat jendela CMD saat di-drag
        terminalText.setPosition(windowFrame.getPosition().x + 12, windowFrame.getPosition().y + 40);
    }
}

void TerminalApp::draw(sf::RenderWindow& window) {
    VirtualWindow::draw(window);
    if (isOpen) {
        window.draw(terminalText);
    }
}