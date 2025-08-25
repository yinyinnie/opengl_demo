# OpenGL Demo Collection

A collection of C++ programs demonstrating various OpenGL concepts using modern OpenGL with GLFW.

## Features

- **Multiple Demo Programs**: Line drawing, triangle rendering, and cube rendering
- Uses OpenGL 3.3 Core Profile
- Implements vertex and fragment shaders
- Clean, modern OpenGL code structure
- CMake-based build system

## Available Demos

- **`draw_line.cpp`**: Draws a red horizontal line across the center of the window
- **`draw_triangle.cpp`**: Renders a colored triangle
- **`draw_cube.cpp`**: Displays a 3D cube with rotation

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
cd opengl_demo
```

2. Use the provided build script:
```bash
./build.sh
```

Or build manually:
```bash
mkdir build
cd build
cmake ..
make
```

## Running

After building, you can run any of the demo programs:

```bash
# Run the line drawing demo
./draw_line

# Run the triangle demo
./draw_triangle

# Run the cube demo
./draw_cube
```

## Controls

- **Line Demo**: Close the window to exit
- **Triangle Demo**: Close the window to exit
- **Cube Demo**: Close the window to exit

## Code Structure

- **`draw_line.cpp`**: Line drawing demo with OpenGL initialization and rendering
- **`draw_triangle.cpp`**: Triangle rendering demo
- **`draw_cube.cpp`**: 3D cube rendering demo
- **`CMakeLists.txt`**: CMake build configuration
- **`build.sh`**: Build script for easy compilation
- **`clean.sh`**: Clean script to remove build artifacts
- **`README.md`**: This file with build and usage instructions

## What Each Demo Does

### Line Demo (`draw_line.cpp`)
1. Initializes GLFW and creates a window
2. Sets up OpenGL context with version 3.3 Core Profile
3. Creates and compiles vertex and fragment shaders
4. Sets up vertex data for a line (two points)
5. Creates vertex array and buffer objects
6. Enters rendering loop that draws the line

### Triangle Demo (`draw_triangle.cpp`)
1. Similar setup to line demo
2. Renders a colored triangle using three vertices
3. Demonstrates basic polygon rendering

### Cube Demo (`draw_cube.cpp`)
1. Sets up 3D rendering context
2. Creates cube geometry with vertices and indices
3. Implements basic 3D transformation matrices
4. Renders a rotating cube

## Build Scripts

- **`build.sh`**: Automatically creates build directory, configures with CMake, and compiles all demos
- **`clean.sh`**: Removes build directory and compiled executables

## Troubleshooting

If you encounter build errors:
- Make sure GLFW3 is properly installed
- Ensure you have a C++17 compatible compiler
- Check that CMake can find the GLFW package

If the programs run but show blank windows:
- Check that your graphics drivers support OpenGL 3.3+
- Verify that the shaders compiled successfully (check console output)

## Development

To add new demos:
1. Create a new `.cpp` file following the existing pattern
2. Add the executable to `CMakeLists.txt`
3. Update this README with the new demo description

## License

This project is open source and available under the MIT License.
