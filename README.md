# Newton Fractal

## Overview
This project visualizes Newton's fractal for the complex function:

f(z) = z^n − 1

It uses ISPC to parallelize the computation across CPU vector lanes for improved performance.  
Rendering and user interaction are implemented using SFML 3.0.

## Requirements
- CMake 3.15 or newer
- C++20 compatible compiler (GCC, Clang)
- ISPC 1.28 or newer
- SFML 3.0.2
- X11 and OpenGL development libraries (required by SFML):
### On Fedora:
```bash
sudo dnf install libXrandr-devel libXcursor-devel libXi-devel libX11-devel \
  mesa-libGL-devel freetype-devel systemd-devel libudev-devel pkgconfig git
```

## Building the Project

### Using CLion
1. Open the project directory in CLion.
2. Ensure that ISPC is available in the system PATH.
3. Build and run the project directly from the IDE.

### Using Terminal
1. Generate build files:
```bash
cmake -S . -B cmake-build-debug -DCMAKE_BUILD_TYPE=Release
```
If ISPC is not in your PATH, specify its location manually:
```bash
cmake -S . -B cmake-build-debug -DCMAKE_BUILD_TYPE=Release -DISPC_EXECUTABLE=/path/to/ispc
```

2. Compile:
```bash
cmake --build cmake-build-debug --config Release
```

The resulting executable should be located in: cmake-build-debug/Newton_Fractal

> Note: This project has been tested and is intended to be built on Linux.

## Gallery
<p align="center">
<img width="4096" height="4096" alt="Newton_Fractal_3" src="https://github.com/user-attachments/assets/39ae75dd-c206-4491-840f-ce85531c94a2" />
</p>



