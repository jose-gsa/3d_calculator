# 3D Mathematical Renderer (from scratch)

A low-level 3D visualization engine for mathematical functions, developed for educational purposes in Linear Algebra and the Modern OpenGL Pipeline (3.3 Core).

## Objectives
The core goal is to understand the "magic" behind 3D rendering by avoiding high-level abstractions and implementing:
- **Custom Math Library:** `Vec3`, `Mat4`, and linear transformations from scratch.
- **MVP Pipeline:** Manual construction of Model, View, and Projection matrices.
- **Shaders:** GLSL programs for hardware-accelerated rendering.
- **Geometry Analytica:** Practical application of vector calculus and coordinate systems.

## Requirements & Dependencies
The project is built for **Linux (Wayland/X11)**.
- **Compiler:** C++17 compatible (GCC/Clang).
- **Build System:** CMake 3.10+.
- **Graphics API:** OpenGL 3.3 Core.
- **Libraries:**
  - `GLFW`: Window management and input.
  - `GLAD`: OpenGL function loader (included in `deps/`).

### On Arch Linux:
```bash
sudo pacman -S cmake glfw-wayland mesa
```
*(Use `glfw-x11` if not using Wayland)*

## Build Instructions
```bash
mkdir build && cd build
cmake ..
make
./GraphicCalculator
```

## Current Status
- [x] OpenGL 3.3 Core Context with GLFW.
- [x] Shader Compilation & Linking Pipeline.
- [x] Basic VAO/VBO Setup.
- [x] Coordinate Axes (X, Y, Z) in NDC space.
- [ ] Proprietarty `math.hpp` implementation.
- [ ] MVP Matrix integration.
- [ ] Function surface triangulation and rendering.

