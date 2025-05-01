# Editor Project

## Overview

This is a basic text editor built using **C++** and **ncurses** for terminal-based applications. It provides a simple interface for text editing with keyboard and mouse support, including essential text manipulation functions like insert, delete, and navigation.

## Features

- **Basic Navigation**:
  - Arrow keys to move the cursor
  - Home/End for line navigation
  - Page Up/Page Down for scrolling
- **Text Editing**:
  - Insert and delete characters
  - Handle special characters (like backspace and enter)
  - Handle mouse events (e.g., click to move cursor)
- **Scrollable Interface**:
  - Automatically scrolls content when the cursor reaches the top/bottom of the screen
  - Dynamic scrolling with the arrow keys
- **Fille managment from within program**:
  - Load and save files.
  - Allows renaming of files


## Requirements

- **C++ compiler** (e.g., g++)
- **ncurses** library for terminal handling

To install `ncurses` on your system:
- **Linux (Ubuntu/Debian)**: `sudo apt-get install libncurses5-dev libncursesw5-dev`
- **macOS**: `brew install ncurses`

## Installation

1. Clone the repository:
   terminal
   git clone project repo link
   cd editor-project