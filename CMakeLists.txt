# ✏️ Terminal Text Editor (TTE)

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![ncurses](https://img.shields.io/badge/ncurses-6.2+-green.svg)
![Platform](https://img.shields.io/badge/Platform-macOS/Linux-lightgrey.svg)
[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](https://yourusername.github.io/your-repo/)

A lightweight, keyboard-focused text editor optimized for full-screen terminal use with advanced features.

## 🚀 Key Features


### ✨ Core Functionality
- **Text Manipulation**
  - Intuitive cursor movement (arrow keys)
  - Screen Scrolling
  - Natural typing support through keyboard
  - Mouse support for cursor positioning
  - Line operations (cut/copy/paste)
  
### 🗂 File Management
- Load/save files with path completion
- Auto-save functionality (configurable)
- Directory navigation and switching
- Save configured settings

### ⚙️ Customization
- Color scheme configuration
- Toggleable line numbers
- Word wrap modes
- Configurable status bar

### 🔄 Advanced Operations
- Full undo/redo history
- clipboard buffer to redo and undo
- Auto save feature
- Line number display 

### 🖥 Terminal Compatibility
This terminal application is designed for fullscreen use
| Terminal       | Full-Screen Support | Notes                                                      |
|----------------|---------------------|------------------------------------------------------------|
| iTerm2         | ✅ Excellent        |   Reccomended Environment                                  |
| Linux Console  | ⚠️ Partial          |   May require different escape codes or may not show colour|

### Prerequisites
- C++17 compatible compiler (Clang 10+/GCC 9+)
- ncurses library (6.2+ recommended)
- CMake (3.30+ Reccomended)
- Doxygen (optional, for documentation)

# macOS
brew install ncurses cmake doxygen


## 💻 Installation & Full-Screen Setup

This is intended to be run in fullscreen view,
Open your terminal and run the following commands:

git clone https://github.com/HarmanB1/Editor.git
cd Editor

mkdir build 
cd build
cmake ..

make

#Documentation 
To view documententation:
Visit blank or run index.html from docs folder

