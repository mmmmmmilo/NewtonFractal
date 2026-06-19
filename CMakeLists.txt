cmake_minimum_required(VERSION 3.19)
# Declare ISPC as a native project language alongside CXX
project(Newton_Fractal LANGUAGES CXX ISPC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
)
FetchContent_MakeAvailable(SFML)

# Explicitly create the directory for ISPC headers to prevent 'fopen' errors.
# The Makefile generator does not always create this directory automatically.
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/ispc_headers")

# Main executable target
add_executable(Newton_Fractal
        src/main.cpp
        src/SetEquation/SetEquation.cpp
        src/SetEquation/SetEquation.h
        src/Scene/Scene.cpp
        src/Scene/Scene.h
        src/SaveImage/SaveImage.cpp
        src/SaveImage/SaveImage.h
        src/FractalImage/FractalImage.cpp
        src/FractalImage/FractalImage.h
        src/FractalCalculation/NewtonFractal.ispc
)

# Configure native ISPC compilation
# Using a relative path for ISPC_HEADER_DIRECTORY to avoid absolute path bugs in Makefiles
set_target_properties(Newton_Fractal PROPERTIES
        ISPC_HEADER_DIRECTORY "ispc_headers"
        ISPC_HEADER_SUFFIX ".h"
        ISPC_INSTRUCTION_SETS "avx2-i32x8"
)

# Include the generated ISPC headers
target_include_directories(Newton_Fractal PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/ispc_headers")

# Link with SFML targets
target_link_libraries(Newton_Fractal
        sfml-graphics
        sfml-window
        sfml-system
)

# Runtime rpath configuration (Linux)
set_target_properties(Newton_Fractal PROPERTIES
        BUILD_RPATH "$ORIGIN"
)

# Copy SFML shared libraries and application resources to the binary directory post-build
add_custom_command(TARGET Newton_Fractal POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory $<TARGET_FILE_DIR:sfml-graphics> $<TARGET_FILE_DIR:Newton_Fractal>
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Newton_Fractal>/Resources
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/src/SetEquation/Mono.ttf $<TARGET_FILE_DIR:Newton_Fractal>/Resources/
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/src/SaveImage/Save.png $<TARGET_FILE_DIR:Newton_Fractal>/Resources/
        COMMENT "Copying SFML shared libraries and graphic resources..."
)
