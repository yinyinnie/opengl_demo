# OpenGL Line Drawing Demo

A simple C++ program that demonstrates how to draw a line using modern OpenGL with GLFW.

## Features

- Uses OpenGL 3.3 Core Profile
- Implements vertex and fragment shaders
- Draws a red horizontal line across the center of the window
- Clean, modern OpenGL code structure

## Prerequisites

- macOS (tested on macOS 10.15+)
- CMake 3.10 or higher
- GLFW3 library
- C++17 compatible compiler (Clang/GCC)

## Installation

### Install GLFW3

Using Homebrew (recommended):
```bash
brew install glfw
```

Or build from source:
```bash
git clone https://github.com/glfw/glfw.git
cd glfw
mkdir build && cd build
cmake ..
make
sudo make install
```

## Building

1. Navigate to the project directory:
```bash
cd /Users/nieyinyin/Code/others/opengl_line_demo
```

2. Create a build directory:
```bash
mkdir build
cd build
```

3. Configure with CMake:
```bash
cmake ..
```

4. Build the project:
```bash
make
```

## Running

After building, run the program:
```bash
./opengl_line_demo
```

You should see a window with a red horizontal line drawn across the center.

## Controls

- Close the window to exit the program
- The line is drawn from left to right across the center of the window

## Code Structure

- `main.cpp`: Main program with OpenGL initialization and rendering loop
- `CMakeLists.txt`: CMake build configuration
- `README.md`: This file with build and usage instructions

## What the Program Does

1. Initializes GLFW and creates a window
2. Sets up OpenGL context with version 3.3 Core Profile
3. Creates and compiles vertex and fragment shaders
4. Sets up vertex data for a line (two points)
5. Creates vertex array and buffer objects
6. Enters rendering loop that draws the line
7. Cleans up resources on exit

## Troubleshooting

If you encounter build errors:
- Make sure GLFW3 is properly installed
- Ensure you have a C++17 compatible compiler
- Check that CMake can find the GLFW package

If the program runs but shows a blank window:
- Check that your graphics drivers support OpenGL 3.3+
- Verify that the shaders compiled successfully (check console output)
