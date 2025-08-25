#include <iostream>
#include <cmath>
#include <GLFW/glfw3.h>
// Modern OpenGL headers
#include <OpenGL/gl3.h>

// ============================================================================
// Matrix Math Utilities
// ============================================================================

// Matrix math utilities
const float pi = M_PI;

struct mat4f {
    // Column-major order
    float
        x11, x21, x31, x41,
        x12, x22, x32, x42,
        x13, x23, x33, x43,
        x14, x24, x34, x44;
};

// Return the given matrix in a format understood by OpenGL.
float* mat4f_gl(struct mat4f* m) {
    // Since it's already in colum-major order, we just return the address of the
    // first element.
    return &m->x11;
}

struct mat4f mat4f_identity = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
};

struct mat4f mat4f_multiply(struct mat4f a, struct mat4f b) {
    return (struct mat4f) {
        a.x11 * b.x11 + a.x12 * b.x21 + a.x13 * b.x31 + a.x14 * b.x41,
        a.x21 * b.x11 + a.x22 * b.x21 + a.x23 * b.x31 + a.x24 * b.x41,
        a.x31 * b.x11 + a.x32 * b.x21 + a.x33 * b.x31 + a.x34 * b.x41,
        a.x41 * b.x11 + a.x42 * b.x21 + a.x43 * b.x31 + a.x44 * b.x41,

        a.x11 * b.x12 + a.x12 * b.x22 + a.x13 * b.x32 + a.x14 * b.x42,
        a.x21 * b.x12 + a.x22 * b.x22 + a.x23 * b.x32 + a.x24 * b.x42,
        a.x31 * b.x12 + a.x32 * b.x22 + a.x33 * b.x32 + a.x34 * b.x42,
        a.x41 * b.x12 + a.x42 * b.x22 + a.x43 * b.x32 + a.x44 * b.x42,

        a.x11 * b.x13 + a.x12 * b.x23 + a.x13 * b.x33 + a.x14 * b.x43,
        a.x21 * b.x13 + a.x22 * b.x23 + a.x23 * b.x33 + a.x24 * b.x43,
        a.x31 * b.x13 + a.x32 * b.x23 + a.x33 * b.x33 + a.x34 * b.x43,
        a.x41 * b.x13 + a.x42 * b.x23 + a.x43 * b.x33 + a.x44 * b.x43,

        a.x11 * b.x14 + a.x12 * b.x24 + a.x13 * b.x34 + a.x14 * b.x44,
        a.x21 * b.x14 + a.x22 * b.x24 + a.x23 * b.x34 + a.x24 * b.x44,
        a.x31 * b.x14 + a.x32 * b.x24 + a.x33 * b.x34 + a.x34 * b.x44,
        a.x41 * b.x14 + a.x42 * b.x24 + a.x43 * b.x34 + a.x44 * b.x44,
    };
}

struct mat4f mat4f_scale(float x, float y, float z) {
    return (struct mat4f){
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1,
    };
}

struct mat4f mat4f_translation(float x, float y, float z) {
    return (struct mat4f) {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1,
    };
}

struct mat4f mat4f_rotate_z(float theta) {
    return (struct mat4f) {
         cos(theta), sin(theta), 0, 0,
        -sin(theta), cos(theta), 0, 0,
                  0,          0, 1, 0,
                  0,          0, 0, 1,
    };
}

struct mat4f mat4f_rotate_y(float theta) {
    return (struct mat4f) {
        cos(theta), 0, -sin(theta), 0,
                 0, 1,           0, 0,
        sin(theta), 0,  cos(theta), 0,
                 0, 0,           0, 1,
    };
}

struct mat4f mat4f_rotate_x(float theta) {
    return (struct mat4f) {
        1, 0, 0, 0,
        0, cos(theta), sin(theta), 0,
        0, -sin(theta), cos(theta), 0,
        0, 0, 0, 1,
    };
}

struct mat4f mat4f_perspective() {
    // Based on http://www.songho.ca/opengl/gl_projectionmatrix.html, which I don't
    // really understand. I just copied the final result.

    const float
        r = 0.5,  // Half of the viewport width (at the near plane)
        t = 0.5,  // Half of the viewport height (at the near plane)
        n = 1,  // Distance to near clipping plane
        f = 5;  // Distance to far clipping plane

    // Note that while n and f are given as positive integers above,
    // the camera is looking in the negative direction. So we will see
    // stuff between z = -n and z = -f.

    return (struct mat4f) {
        n / r, 0, 0, 0,
        0, n / t, 0, 0,
        0, 0, (-f - n) / (f - n), -1,
        0, 0, (2 * f * n) / (n - f), 0,
    };
}

// Shader source code embedded as raw string literals
const char* vertex_shader_source = R"(
#version 330
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 vertex_color;

uniform mat4 transform;

out vec3 color;

void main() {
    gl_Position = transform * vec4(pos, 1.0);
    color = vertex_color;
}
)";

const char* fragment_shader_source = R"(
#version 330

in vec3 color;

out vec4 frag_color;

void main() {
    frag_color = vec4(color, 1.0);
}
)";

struct context {
    unsigned int shader_program;
    unsigned int vao;
    GLFWwindow* window;
    unsigned int uniform_transform;
};

void render(struct context*);
void initialize(struct context*);

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set OpenGL context hints for macOS compatibility
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a window
    GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL Cube Demo", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    
    // Set viewport
    glViewport(0, 0, 800, 800);
    
    glfwSwapInterval(0);
    // GLEW initialization removed - using modern OpenGL

    // ============================================================================
    // Main Application Logic
    // ============================================================================

    struct context context;
    context.window = window;
    initialize(&context);

    while (!glfwWindowShouldClose(window)) {
        render(&context);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glDeleteVertexArrays(1, &context.vao);
    glDeleteProgram(context.shader_program);
    
    glfwTerminate();
    return 0;
}

// Function to compile shader
unsigned int compileShader(unsigned int type, const char* source) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    // Check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    return shader;
}

// Function to create shader program
unsigned int createShaderProgram() {
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertex_shader_source);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragment_shader_source);
    
    // Create shader program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // Check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    // Delete shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}

// ============================================================================
// Constants and Data
// ============================================================================

const unsigned int triangles = 6 * 2;   // Number of triangles rendered

const unsigned int verticies_index = 0;
const unsigned int colors_index = 1;

// ============================================================================
// Initialization Functions
// ============================================================================

void initialize(struct context* context) {
    glEnable(GL_DEPTH_TEST);

    float vertices[] = {
        // Front face
         0.5,  0.5,  0.5,
        -0.5,  0.5,  0.5,
        -0.5, -0.5,  0.5,
         0.5, -0.5,  0.5,

        // Back face
         0.5,  0.5, -0.5,
        -0.5,  0.5, -0.5,
        -0.5, -0.5, -0.5,
         0.5, -0.5, -0.5,
    };

    float vertex_colors[] = {
        1.0, 0.4, 0.6,
        1.0, 0.9, 0.2,
        0.7, 0.3, 0.8,
        0.5, 0.3, 1.0,

        0.2, 0.6, 1.0,
        0.6, 1.0, 0.4,
        0.6, 0.8, 0.8,
        0.4, 0.8, 0.8,
    };

    unsigned short triangle_indices[] = {
        // Front
        0, 1, 2,
        2, 3, 0,

        // Right
        0, 3, 7,
        7, 4, 0,

        // Bottom
        2, 6, 7,
        7, 3, 2,

        // Left
        1, 5, 6,
        6, 2, 1,

        // Back
        4, 7, 6,
        6, 5, 4,

        // Top
        5, 1, 0,
        0, 4, 5,
    };

    glGenVertexArrays(1, &context->vao);
    glBindVertexArray(context->vao);

    unsigned int triangles_ebo;
    glGenBuffers(1, &triangles_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangles_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof triangle_indices, triangle_indices, GL_STATIC_DRAW);

    unsigned int verticies_vbo;
    glGenBuffers(1, &verticies_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, verticies_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(verticies_index, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(verticies_index);

    unsigned int colors_vbo;
    glGenBuffers(1, &colors_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof vertex_colors, vertex_colors, GL_STATIC_DRAW);

    glVertexAttribPointer(colors_index, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(colors_index);

    // Unbind to prevent accidental modification
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Create shader program using standardized approach
    context->shader_program = createShaderProgram();

    context->uniform_transform = glGetUniformLocation(context->shader_program, "transform");
}

// Based on https://antongerdelan.net/opengl/glcontext2.html
void update_fps(struct context* context) {
    // All times in seconds

    static double last_update_time = 0;
    static int frames_since_last_update = 0;

    double now = glfwGetTime();
    frames_since_last_update++;

    if (now - last_update_time > 0.25) {
        double fps = frames_since_last_update / (now - last_update_time);

        char title_buffer[128];
        snprintf(title_buffer, sizeof(title_buffer), "Cube (%.1f FPS)", fps);
        glfwSetWindowTitle(context->window, title_buffer);

        last_update_time = now;
        frames_since_last_update = 0;
    }
}

float animation(float duration) {
    unsigned long int ms_time = glfwGetTime() * 1000;
    unsigned int ms_duration = duration * 1000;
    float ms_position = ms_time % ms_duration;

    return ms_position / ms_duration;
}

void render(struct context* context) {
    update_fps(context);

    // Clear
    {
        glClearColor(0.1, 0.12, 0.2, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    glUseProgram(context->shader_program);

    struct mat4f transform = mat4f_identity;
    transform = mat4f_multiply(transform, mat4f_perspective());
    transform = mat4f_multiply(transform, mat4f_translation(0, 0, -3));
    transform = mat4f_multiply(transform, mat4f_rotate_x(0.15 * pi));
    transform = mat4f_multiply(transform, mat4f_rotate_y(2 * pi * animation(4)));
    glUniformMatrix4fv(context->uniform_transform, 1, GL_FALSE, mat4f_gl(&transform));

    glBindVertexArray(context->vao);
    glDrawElements(GL_TRIANGLES, triangles * 3, GL_UNSIGNED_SHORT, NULL);
}
