cmake_minimum_required(VERSION 3.10)
project(GraphicCalculator)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

# Include header directories
include_directories(include deps/glad/include)

# Find GLFW installed on the system
find_package(glfw3 REQUIRED)

# Add GLAD source code (needs to be compiled together)
add_executable(${PROJECT_NAME} 
    src/main.cpp 
    src/engine.cpp
    deps/glad/src/glad.c
)

# Link with necessary libraries
target_link_libraries(${PROJECT_NAME} glfw GL dl)
