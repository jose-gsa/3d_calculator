#include "engine.hpp"
#include <iostream>

int main() {
  // Instantiate our Engine class
  Engine engine;

  if (!engine.init(800, 600, "Graphic Calculator 3D")) {
    std::cerr << "Error creating window\n";
    return -1;
  }

  engine.run();

  return 0;
}
