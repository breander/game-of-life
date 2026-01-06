# Game of Life

A graphical implementation of Conway's Game of Life using the Allegro5 graphics library.

## Description

This is a visual simulation of [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life), a cellular automaton devised by mathematician John Conway. The simulation creates evolving patterns on a grid where cells live, die, or reproduce based on simple rules.

### Game of Life Rules

The simulation follows the classic Conway's Game of Life rules:

1. **Survival**: A live cell with 2 or 3 live neighbors survives to the next generation
2. **Death**: A live cell with fewer than 2 neighbors dies (underpopulation), or with more than 3 neighbors dies (overpopulation)
3. **Birth**: A dead cell with exactly 3 live neighbors becomes alive (reproduction)

### Features

- Real-time visualization with configurable display settings
- Randomly generated initial patterns
- FPS counter display
- Cells that became alive are highlighted in red
- Live cells are displayed in white
- Configurable window size, FPS, and cell block size

## Dependencies

This project requires the Allegro5 game programming library and its addons:

- **allegro5** - Core Allegro library
- **allegro_main** - Main entry point addon
- **allegro_font** - Font rendering addon
- **allegro_ttf** - TrueType font support
- **allegro_primitives** - Primitive drawing functions
- **allegro_color** - Color manipulation utilities

### Installing Dependencies

On Ubuntu/Debian:
```bash
sudo apt-get install liballegro5-dev liballegro5-2 liballegro-ttf5-dev liballegro-primitives5-dev
```

On macOS (using Homebrew):
```bash
brew install allegro
```

On Fedora:
```bash
sudo dnf install allegro5-devel allegro5-addon-ttf-devel allegro5-addon-primitives-devel
```

## Building

The project uses a makefile for compilation. To build the application:

```bash
make
```

This will compile the source code and create an executable named `main`.

To clean build artifacts:
```bash
make clean
```

## Usage

### Basic Usage

Run the application with default settings (1920x1080 resolution, 60 FPS, 8-pixel blocks):

```bash
./main
```

### Command-Line Options

The application supports several command-line arguments to customize the simulation:

- `-width <pixels>` - Set the window width (default: 1920)
- `-height <pixels>` - Set the window height (default: 1080)
- `-fps <number>` - Set the target frames per second (default: 60)
- `-debug` - Enable debug mode

#### Examples

Run with a smaller window at 30 FPS:
```bash
./main -width 1280 -height 720 -fps 30
```

Run in debug mode with custom resolution:
```bash
./main -width 1600 -height 900 -debug
```

## Controls

- **Enter** - Generate a new random pattern
- **Escape** - Exit the application
- **Close Window** - Exit the application

## Project Structure

```
.
├── main.cpp              # Main source code file
├── makefile              # Build configuration
├── OpenSans-Regular.ttf  # Font file for text rendering
└── README.md             # This file
```

### Code Overview

The `main.cpp` file contains:
- Allegro5 initialization and setup
- Command-line argument parsing
- Game board representation using a boolean array
- Game of Life logic implementation with neighbor counting
- Rendering loop with FPS display
- Event handling for keyboard input

The simulation divides the screen into a grid of blocks (default 8x8 pixels each), with each block representing one cell in the Game of Life.

## Technical Details

- **Language**: C++ (C++14 standard)
- **Graphics Library**: Allegro5
- **Grid Size**: Calculated dynamically based on window size and block size
- **Initial State**: Randomly generated (approximately 50% alive cells)
- **Color Coding**: 
  - White: Live cells that maintained their alive state
  - Red: Cells that became alive in the current generation
  - Black: Dead cells (background)

## License

This project's license information is not specified.

## Author

breander
