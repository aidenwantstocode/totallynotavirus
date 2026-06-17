# TotallyNotAVirus - Virtual OS Simulation Style Survival Game

[![C++ Version](https://img.shields.io/badge/C%2B%2B-17%2F20-blue.svg)](https://en.cppreference.com/)
[![Framework](https://img.shields.io/badge/Framework-SFML%202.6.1-green.svg)](https://www.sfml-dev.org/)
[![Build System](https://img.shields.io/badge/Build%20System-CMake-orange.svg)](https://cmake.org/)
[![Environment](https://img.shields.io/badge/Environment-MSYS2%20(UCRT64)-cyan.svg)](https://www.msys2.org/)

### About The Game

> "While scrambling to clean up things in your childhood home, you found a dusty, forgotten flash drive hidden beneath all the junk in your decrepit basement. Slightly hoping for potential childhood memories stored within the drive, you plug it into an aged family computer you haven't used since high-school, only to find out that the little flash drive isn't as harmless as you thought it was."

***TotallyNotAVirus*** is an interactive, OS Simulator style survival game heavily inspired by *KinitoPET* and *Welcome To The Game*. You will try to survive against certain *Corruptions* in an old Windows 98-style system, trying to preserve hidden encrypted data inside of an old, retaliating flash drive while fighting to keep your old computer alive.

The game isn't projected to have too many scary elements relentlessly dished out in front of the players, though this might change in the upcoming future based on changes in creative direction and technical decisions. Even with this, the core gameplay dynamics will still feature fast-paced periodically initiated waves of threats as well as active player defense mechanisms of the game environment. 

The game is currently being actively developed on C++ using SFML. Be sure to check this repository once in a while if you're interested!

### Promised Features (The Player's Experience)
* **True-to-Life Simulation:** A fully stable replica mimicking an old, mid-90s operating system complete with responsive button clicks, smooth desktop navigation, and standard desktop apps.
* **The "Infection" Trigger Event:** A seamless, prompted transition from a safe simulation environment into an active fight-or-flight scenario triggered by an in-game event.
* **Smart Malware Behavior:** Rather than static pre-rendered jumpscares, the *Corruptions* in the system interacts dynamically with the player's actions- forcefully spawning popups, shifting windows away from the mouse cursor, and messing with application text inputs.
* **Hacker Survival Mechanics:** Players will utilize various forms of defense mechanics in order to preserve the computer's health from dropping. *Corruptions* will relentlessly try to nuke the computer with various types of attacks ranging from memory overflows to indestructable pop-ups.
* **Integrated Play-Style Adjuster:** A seemless, non-direct, and lore-friendly difficulty/play-style settings integrated through an in-game event that uses a *"speed or safety"* philosophy. Players can either choose to play the game slowly and carefully with more ways to manage threats, or speed through the game with narrower error margins.
* **Quality-of-Life Features:** QoL features like a settings menu consisting of adjustable graphics, gameplay-oriented options, and accesibility settings are very much planned to be integrated seemlessly into the immediate game environment (i.e. in the form of a desktop app inside of the game itself).
* **Performance Insights:** The game is projected to be quite light to run on modern Windows machines, though extensive testing is needed to achieve this conclusion.

*More entries will be added to the list as development progresses.

### Repository Structure

* `include/` - Header files (`.hpp`) outlining game core and virtual app blueprints.
* `src/` - Implementation files (`.cpp`) managing SFML renders and OS mechanics.
* `CMakeLists.txt` - Build configuration system.

