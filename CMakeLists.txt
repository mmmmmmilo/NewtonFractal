cmake_minimum_required(VERSION 3.15)
project(Newton_Fractal)

set(CMAKE_CXX_STANDARD 20)           # require C++20
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)                # allow fetching SFML if not present

set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)   # disable SFML audio build
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
)
FetchContent_MakeAvailable(SFML)     # download/build SFML if needed

# Try to find 'ispc' in PATH and store it in the cache
find_program(_FOUND_ISPC ispc)
if(_FOUND_ISPC)
    set(ISPC_EXECUTABLE "${_FOUND_ISPC}" CACHE FILEPATH "Path to the ISPC executable (ispc)." FORCE)
endif()

# Fail early if ISPC not found
if(NOT ISPC_EXECUTABLE OR ISPC_EXECUTABLE STREQUAL "")
    message(FATAL_ERROR "ISPC_EXECUTABLE is not set. Install 'ispc' into your PATH so CMake can find it automatically.")
endif()

# where ISPC generates outputs
set(ISPC_OUTPUT_DIR "${CMAKE_BINARY_DIR}/ispc")
file(MAKE_DIRECTORY ${ISPC_OUTPUT_DIR})

# compile ISPC source to object + header
add_custom_command(
        OUTPUT
        ${ISPC_OUTPUT_DIR}/NewtonFractal.o
        ${ISPC_OUTPUT_DIR}/NewtonFractal.h
        COMMAND ${ISPC_EXECUTABLE} ${CMAKE_SOURCE_DIR}/src/FractalCalculation/NewtonFractal.ispc
        -o ${ISPC_OUTPUT_DIR}/NewtonFractal.o
        -h ${ISPC_OUTPUT_DIR}/NewtonFractal.h
        --target=avx2-i32x8
        DEPENDS ${CMAKE_SOURCE_DIR}/src/FractalCalculation/NewtonFractal.ispc
        COMMENT "Compiling ISPC: NewtonFractal.ispc"
)

# static library wrapping the generated ISPC object
add_library(NewtonFractalISPC STATIC ${ISPC_OUTPUT_DIR}/NewtonFractal.o)
set_target_properties(NewtonFractalISPC PROPERTIES LINKER_LANGUAGE CXX)

# main executable and sources
add_executable(Newton_Fractal src/main.cpp
        src/SetEquation/SetEquation.cpp
        src/SetEquation/SetEquation.h
        src/Scene/Scene.cpp
        src/Scene/Scene.h
        src/SaveImage/SaveImage.cpp
        src/SaveImage/SaveImage.h
        src/FractalImage/FractalImage.cpp
        src/FractalImage/FractalImage.h
)

# include ISPC-generated headers when building the executable
target_include_directories(Newton_Fractal PRIVATE ${ISPC_OUTPUT_DIR})

# link SFML and the ISPC static lib
target_link_libraries(Newton_Fractal
        sfml-graphics
        sfml-window
        sfml-system
        NewtonFractalISPC
)

# runtime rpath so shared libs are searched relative to the exe
set_target_properties(Newton_Fractal PROPERTIES
        BUILD_RPATH "$ORIGIN"
)

# copy SFML shared libraries next to the executable after build
add_custom_command(TARGET Newton_Fractal POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        $<TARGET_FILE_DIR:sfml-graphics>
        $<TARGET_FILE_DIR:Newton_Fractal>
)

# copy resources into build Resources directory
file(COPY ${CMAKE_SOURCE_DIR}/src/SetEquation/Mono.ttf
        DESTINATION ${CMAKE_BINARY_DIR}/Resources)
file(COPY ${CMAKE_SOURCE_DIR}/src/SaveImage/Save.png
        DESTINATION ${CMAKE_BINARY_DIR}/Resources)
