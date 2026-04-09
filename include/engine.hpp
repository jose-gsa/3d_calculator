#pragma once

// The inclusion order is VITAL in modern OpenGL:
// GLAD MUST be included BEFORE GLFW.
// GLAD embeds and loads the OpenGL headers that GLFW needs to use under the hood.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Engine {
public:
  Engine();
  ~Engine();

  // Initializes the window and graphic context.
  // Returns true if everything goes well, false otherwise.
  bool init(int width, int height, const char *title);

  // Contains the Main Loop of the application.
  // Should only be called if init() returns true.
  void run();

private:
  // Final cleanup of memory resources, window manager, and OpenGL.
  void cleanup();

  // --- NEW STAGE: GRAPHIC PIPELINE (SHADERS AND BUFFERS) --- //

  // Compiles a single shader (Vertex or Fragment) and returns its ID (handle).
  unsigned int compileShader(unsigned int type, const char* source);
  
  // Receives the source code of both shaders, compiles and links them into a final Program.
  unsigned int createShaderProgram(const char* vertexSource, const char* fragmentSource);
  
  // Function to create the VAO and VBO of our XYZ axes and send them to the GPU.
  void setupAxesData();

  // Opaque pointer to the window manager structure (GLFW).
  GLFWwindow *window;

  // Handles (IDs) that OpenGL gives back to us to reference resources in VRAM.
  unsigned int shaderProgram; // Our GPU program
  unsigned int VAO;           // Vertex Array Object (Stores the memory configuration)
  unsigned int VBO;           // Vertex Buffer Object (Stores the raw bytes/floats)
};
