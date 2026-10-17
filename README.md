<div align="center">

# Galaxy Wars

A 2D arcade space shooter in C++ and SFML with three levels, power-ups, a boss and a saved leaderboard.

![C++](https://img.shields.io/badge/C%2B%2B-20-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![SFML](https://img.shields.io/badge/SFML-2.5.1-8CC445?style=flat-square)
![Visual Studio](https://img.shields.io/badge/Visual%20Studio-2022-5C2D91?style=flat-square&logo=visualstudio&logoColor=white)
![Platform](https://img.shields.io/badge/Platform-Windows-0078D4?style=flat-square&logo=windows&logoColor=white)

</div>

## Overview

Galaxy Wars is a 2D arcade shooter for Windows. You fly a ship around an 800x600 field, shoot down waves of enemies, dodge asteroids and enemy fire, and pick up power-ups while the difficulty rises across three levels. It is written in C++ on top of SFML and is organised as a small object-oriented game engine: an abstract game-object base class, polymorphic updates, a template container that owns the live objects, and a state machine that drives ten screens from splash to victory.

## Demo

Screenshots of every screen (splash, menu, name entry, instructions, leaderboard, all three levels, power-ups, pause, game over and victory) are in section 4 of [Project Report.pdf](Project%20Report.pdf). The class diagram is in [UML.pdf](UML.pdf).

## Key Features

- **Three levels driven by score.** Level 2 starts at 500 points and level 3 at 1500. Enemy speed (2.0, 3.0, 3.5) and spawn interval (1.0 s, 0.6 s, 0.3 s) increase per level, with a 5 second transition screen between levels. Surviving 120 seconds in level 3 wins the game.
- **Four enemy types.** Basic enemies fall straight down, fast enemies appear from level 2, pattern enemies move on a sine wave and shoot back in level 3, and a 100 HP boss spawns once in level 3 after 2000 points.
- **Formation waves.** Every 8 seconds the game spawns a row, a V shape or a diagonal line of enemies on top of the regular random spawns, plus falling asteroids.
- **Three power-ups.** Shield (5 seconds of invulnerability with a pulsing bubble), Fire Rate (0.08 second fire cooldown for 5 seconds with a rotating ring effect) and Repair (+30 HP with a green particle burst).
- **Manual and automatic fire.** Space shoots, and F toggles auto-fire. Bullets inherit the ship's horizontal velocity.
- **Persistent leaderboard.** Scores are saved to a text file (top 50, bubble-sorted, top 10 shown in game) and appended to a binary file with length-prefixed names.
- **Music and screens.** Four OGG tracks switch with the game state through SFML audio: menu and gameplay loop, game over and victory play once.
- **Object-oriented design.** Abstract `GameObject` with a pure virtual `update()`, a `GameObjectArray<T>` template container, operator overloading in `Resource`, a custom `GameException`, and axis-aligned bounding-box collision.

## Tech Stack

| Area | Technology |
|---|---|
| Language | C++ (C++20 in the project settings) |
| Graphics, window, input, audio | SFML 2.5.1 (graphics, window, system and audio modules) over OpenAL |
| Build | Visual Studio 2022 solution (MSVC toolset v143), x64 |
| Assets | PNG and JPEG sprites, OGG music, Skranji TTF font |
| Persistence | `scores.txt` (text) and `scores.dat` (binary) through C++ file streams |
| Documentation | UML class diagram and a design write-up, both as PDF |

## Project Structure

```text
galaxy-wars-game/
  GalaxyWars.sln             Visual Studio solution
  GalaxyWars/
    main.cpp                 Entry point: seeds the RNG, runs the engine, prints saved binary scores on exit
    GameEngine.h             Game loop, 10-state screen machine, spawning, collisions, rendering, music
    GameObject.h             Abstract base class for everything drawn on screen
    Ships.h                  Ship, Player, enemy types, projectiles and particle effects
    GameObjects.h            Asteroid and the three PowerUp types
    Managers.h               LevelManager (difficulty by score) and ScoreManager (text and binary scores)
    GameObjectArray.h        Template container that updates, draws and frees game objects
    BoundingBox.h            Axis-aligned collision rectangle
    Resource.h               Score value type with operator overloads
    GameException.h          Custom exception used for score file errors
    *.dll                    SFML and OpenAL runtime libraries (x64, release and debug)
  Sprites/  Music/  arial/   Images, OGG tracks and the Skranji font, loaded by relative path
  UML.pdf  Project Report.pdf
```

## Getting Started

### Prerequisites

- Windows (64-bit)
- Visual Studio 2022 with the Desktop development with C++ workload
- SFML 2.5.1 for 64-bit Visual C++, extracted to `C:\SFML\SFML-2.5.1` (the project file expects `include` and `lib` directly under that folder; change Additional Include Directories and Additional Library Directories in the project properties to use another location)

No environment variables are required.

### Installation

```bash
git clone https://github.com/shahramshafiq/galaxy-wars-game.git
cd galaxy-wars-game
```

Open `GalaxyWars.sln` in Visual Studio, select the **Debug** configuration and the **x64** platform, then choose Build > Build Solution. Only the x64 configurations have the SFML paths set, and the Release configuration is currently set to link the debug SFML libraries, so Debug x64 is the one to use.

Copy the runtime libraries next to the built executable:

```bat
copy GalaxyWars\sfml-*-d-2.dll x64\Debug\
copy GalaxyWars\openal32.dll x64\Debug\
```

### Run

The game loads `Sprites/`, `Music/` and `arial/` by relative path, so it must start with the repository root as the working directory. From the repository root:

```bat
x64\Debug\GalaxyWars.exe
```

To run from Visual Studio instead, set Project > Properties > Debugging > Working Directory to `$(SolutionDir)`.

## Usage

| Screen | Keys |
|---|---|
| Main menu | `1` Start Game, `2` High Scores, `3` Instructions, `4` Exit |
| Name entry | Letters, digits and `_` (up to 15 characters), `Enter` to start, `Esc` to go back |
| Playing | Arrow keys move, `Space` shoots, `F` toggles auto-fire, `P` pauses, hold `D` to print debug info to the console |
| Paused | `1` resume, `2` back to the main menu |
| Game over or victory | `Esc` back to the main menu, `X` exit |

Your score is written to `scores.txt` and `scores.dat` in the working directory when you die or win. The leaderboard screen reads `scores.txt`. When the window closes, the console prints the entries stored in `scores.dat`.

## Limitations and Roadmap

- **One run per launch.** `GameEngine::resetGame()` is defined but never called, so after a game over or a victory the player and level state are not reset. Restart the program to play again.
- **Wave-spawned pattern enemies do not shoot.** They are created without an enemy-bullet list, so only the individually spawned pattern enemies fire back.
- **Build setup.** The SFML path is hard-coded to `C:\SFML\SFML-2.5.1`, and the Release x64 configuration links the debug SFML libraries. Using an environment variable for the SFML path and fixing the Release libraries would make the build portable.
- **No automated tests.** Game logic lives in header files and is checked by playing the game.

## Author

**Shahram Shafiq** - [GitHub](https://github.com/shahramshafiq) | [LinkedIn](https://www.linkedin.com/in/shahramshafiq/)
