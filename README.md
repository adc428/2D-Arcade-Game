# Project: Text Adventure World in C++

# Description

This project implements a console-based Text Adventure World game in C++.  
The game features two cooperative players moving through multiple rooms, interacting with doors, keys, springs, bombs, switches, riddles, and obstacles.  
The world is loaded from external screen files, and the game uses text-based rendering inside a standard 80x25 console window.

# Components

Game Manager (`Game`):
Controls the main menu, game loop, room transitions, lives system, and overall game flow.

Screen System (`Screen`):
Loads and displays rooms from `.screen` files and manages the game board content.

Players (`Player`):
Handles movement, input, item holding, collisions, and interaction with world elements.

Doors and Keys (`Door`, door logic in `Game`):
Supports locked doors, key usage, and transitions between rooms.

Springs (`Spring`):
Implements spring-based movement and acceleration mechanics.

Bombs (`Bomb`):
Allows collecting, placing, activating, and exploding bombs.

Switches (`Switch`):
Supports switch activation and door-related logic.

Riddles (`Riddle`, `Questions`):
Displays riddles and checks player answers before allowing progress.

Obstacles (`Obstacle`):
Represents pushable or blocking elements in the world.

User Interface (`GameUI`):
Shows the main menu, instructions, and game messages.

Utilities (`utils`):
Provides helper console functions such as cursor positioning and screen control.

# Features

Two-player cooperative gameplay.

Multiple rooms loaded from external screen files.

Support for keys, doors, springs, bombs, switches, riddles, and obstacles.

Lives system and room progression.

Pause menu during gameplay.

Legend/status display on screen.

Riddle loading from `riddles.txt`.

# Files

## Source Files
- `Main.cpp`
- `Game.cpp`, `Game.h`
- `Screen.cpp`, `Screen.h`
- `Player.cpp`, `Player.h`
- `Door.cpp`, `Door.h`
- `Game_door.cpp`
- `Spring.cpp`, `Spring.h`
- `Bomb.cpp`, `Bomb.h`
- `Switch.cpp`, `Switch.h`
- `Obstacle.cpp`, `Obstacle.h`
- `Riddle.cpp`, `Riddle.h`
- `Questions.cpp`, `Questions.h`
- `Point.cpp`, `Point.h`
- `GameUI.cpp`, `GameUI.h`
- `utils.cpp`, `utils.h`
- `Direction.h`

## Game Data Files
- `adv-world_01.screen`
- `adv-world_02.screen`
- `adv-world_03.screen`
- `riddles.txt`

# Requirements

- C++ compiler
- Visual Studio 2022 or later
- Windows console environment
- Standard C++ libraries
- Windows-specific console support (`gotoxy`, `_kbhit`, `_getch`, `Sleep`)

# How to Run

Open the project in Visual Studio and run the program from the IDE.

Make sure the following files are in the working directory:
- `adv-world_01.screen`
- `adv-world_02.screen`
- `adv-world_03.screen`
- `riddles.txt`

# Notes

This project is based on the Text Adventure World course assignment, which requires a console game with two players, multiple rooms, external screen files, and interactive game elements. :contentReference[oaicite:1]{index=1} :contentReference[oaicite:2]{index=2} :contentReference[oaicite:3]{index=3}
