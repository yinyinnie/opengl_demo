cmake_minimum_required(VERSION 3.10)
project(OpenGLDemos)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find GLFW
find_package(glfw3 REQUIRED)

# Add executable for line drawing
add_executable(opengl_line_demo draw_line.cpp)
target_link_libraries(opengl_line_demo glfw)

# Add executable for triangle drawing
add_executable(opengl_triangle_demo draw_triangle.cpp)
target_link_libraries(opengl_triangle_demo glfw)

# Add executable for cube drawing
add_executable(opengl_cube_demo draw_cube.cpp)
target_link_libraries(opengl_cube_demo glfw)

# Link macOS frameworks for OpenGL
if(APPLE)
    target_link_libraries(opengl_line_demo "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
    target_link_libraries(opengl_triangle_demo "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
    target_link_libraries(opengl_cube_demo "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
endif()
