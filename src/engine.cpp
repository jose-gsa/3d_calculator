#include "engine.hpp"
#include <GLFW/glfw3.h>
#include <cstddef>
#include <iostream>

// Literal source codes of the Shaders (GLSL - OpenGL Shading Language)
// In an advanced engine, you would read this from .glsl or .vert/.frag files.
// For now, "hardcoded" is the didactic way to understand the process.

const char *vertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 aPos;   // Attribute 0 will be the Position (X, Y, Z)
    layout (location = 1) in vec3 aColor; // Attribute 1 will be the Color (R, G, B)

    out vec3 vertexColor; // Output variable to pass the color to the Fragment Shader

    void main() {
        // gl_Position is a native OpenGL variable that tells WHERE the vertex is on the screen.
        // The fourth component (1.0) is "W", used for projection and perspective math (we will see later).
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
        vertexColor = aColor; // Forwards the color
    }
)glsl";

const char *fragmentShaderSource = R"glsl(
    #version 330 core
    in vec3 vertexColor; // Input variable coming from the Vertex Shader (interpolated!)
    
    out vec4 FragColor;  // The final color of the pixel on the screen

    void main() {
        // Defines the pixel color. The fourth component is Alpha (transparency, 1.0 = fully opaque).
        FragColor = vec4(vertexColor, 1.0);
    }
)glsl";

Engine::Engine() : window(nullptr), shaderProgram(0), VAO(0), VBO(0) {}

Engine::~Engine() { cleanup(); }

bool Engine::init(int width, int height, const char *title) {
  if (!glfwInit()) {
    std::cerr << "Error initializing GLFW\n";
    return false;
  }

  // window hints configuration
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create window
  window = glfwCreateWindow(width, height, title, NULL, NULL);
  if (!window) {
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    return false;

  glViewport(0, 0, width, height);

  // --- NEW INTEGRATION IN INIT --- //

  // 1. Create Shader Program
  shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
  if (shaderProgram == 0) {
    return false;
  }

  // 2. Setup axes data
  setupAxesData();

  return true; // Success!
}

void Engine::run() {
  if (!window)
    return;

  // Main verification/Draw loop
  while (!glfwWindowShouldClose(window)) {

    // Background color
    glClearColor(0.02f, 0.02f, 0.02f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 6);

    // Unbind VAO as a good practice
    glBindVertexArray(0);

    // Buffer processor
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

unsigned int Engine::compileShader(unsigned int type, const char *source) {

  // Create shader
  GLuint id = glCreateShader(type);

  glShaderSource(id, 1, &source, NULL);

  // Compile
  glCompileShader(id);

  // ERROR handling
  GLint success;
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);

  if (success != GL_TRUE) {
    GLsizei log_length = 0;
    GLchar message[1024];
    glGetShaderInfoLog(id, 1024, &log_length, message);

    std::cerr << "Shader compilation failed: " << message << "\n";
    return 0;
  }

  return id;
}

unsigned int Engine::createShaderProgram(const char *vertexSource,
                                         const char *fragmentSource) {

  GLuint vshader = compileShader(GL_VERTEX_SHADER, vertexSource);
  GLuint fshader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

  // Linker
  GLuint program = glCreateProgram();
  glAttachShader(program, vshader);
  glAttachShader(program, fshader);
  glLinkProgram(program);

  glDeleteShader(vshader);
  glDeleteShader(fshader);

  return program;
}

void Engine::setupAxesData() {

  // Define NDC
  float vertices[] = {
      // POSITIONS (X, Y, Z)    // COLORS (R, G, B)
      // X Axis (Red)
      -0.8f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.8f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

      // Y Axis (Green)
      0.0f, -0.8f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.8f, 0.0f, 0.0f, 1.0f, 0.0f,

      // Z Axis (Blue) - Fake Isometric (Without Camera/Projection math,
      // drawn diagonally)
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f};

  // ID generator
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Read array - positions
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Read array - colors
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Engine::cleanup() {
  // --- Cleanup of OpenGL resources ---
  if (VAO != 0)
    glDeleteVertexArrays(1, &VAO);
  if (VBO != 0)
    glDeleteBuffers(1, &VBO);
  if (shaderProgram != 0)
    glDeleteProgram(shaderProgram);

  if (window != NULL) {
    glfwDestroyWindow(window);
    glfwTerminate();
  }
}
