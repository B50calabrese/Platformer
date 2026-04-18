# Project: Platformer

A game similar to RoguelikeCardgame, built on top of the GameEngine.

## Project Structure

- `src/`: Contains all source files (`.cpp`).
- `include/`: Contains all header files (`.h`). The structure here mirrors the `src/` directory.
- `libs/`: External libraries and submodules.
  - `libs/GameEngine`: The core game engine.
- `assets/`: Game assets like textures, sounds, and fonts.

## Current Project Status

- **Scenes Implementation**:
  - Basic scene skeleton established in `include/scenes/` and `src/scenes/`.
  - Available scenes: `MainMenuScene`, `LevelSelectScene`, and `LevelScene`.
  - `MainMenuScene` is the initial scene set in `src/main.cpp`.

## Coding Standards

This project follows the **Google C++ Style Guide**.

### Key Rules for AI Collaborators

1.  **Strict Google C++ Style**: Braces for all control structures. Pointers for output parameters.
2.  **Naming Conventions**:
    - **PascalCase** for types and functions.
    - **snake_case** for local variables and parameters.
    - **Trailing underscores** for private/protected class members (`member_variable_`).
    - **kPascalCase** for constants and enumerators (e.g., `kMaxSpeed`).
3.  **Directory Naming**: Use purely alphabetical characters for directory names (e.g., `gamescenes` instead of `game_scenes`). Files can use underscores.
4.  **No `using namespace`**: Forbidden in headers. Avoid in source files; use explicit qualifiers.
5.  **Formatting**: Use the provided `.clang-format` file.

## Build Instructions

- Standard Build: `mkdir -p build && cd build && cmake .. && cmake --build .`