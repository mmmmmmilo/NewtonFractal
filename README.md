# Interactive Newton Fractal Generator
A high-performance, interactive application for rendering Newton fractals in real-time. Developed using **C++20**, **Intel ISPC**, and **SFML 3.0**, this project leverages hardware-level vectorization to dynamically visualize the complex plane and explore the convergence of the Newton-Raphson method.

## Description
The visualizer maps convergence to roots of the complex equation $f(z) = z^n - 1$ using Newton's iterative method:

$$z_{k+1} = z_k - \frac{f(z_k)}{f'(z_k)}$$

The coordinate of each pixel is mapped to a complex number $z_0$. The renderer tracks which root of unity the series converges to (which determines the pixel's hue) and how many iterations were required to reach convergence (which determines the pixel's brightness).

## Highlights & Implementation Details
* **SIMD Accelerated Computation:** Powered by the Intel Implicit SPMD Program Compiler (ISPC). The fractal calculation is compiled into optimized SIMD instructions (targeting AVX2), enabling vector-parallel execution across multiple pixels simultaneously on the CPU.
* **Auto-Normalization Contrast:** Dynamically scans the iteration field to maximize contrast and aesthetic rendering quality, adapting instantly across different polynomial powers.
* **High-Resolution Exporter:** Instead of just dumping the visible window buffer (1024x1024), the application runs a completely separate calculation pass at 4096x4096 resolution when exporting an image. This ensures maximum geometric detail for the saved fractals without wasting CPU cycles on rendering unnecessary pixels during real-time exploration.
* **Real-Time Equation Editing:** Features a custom, smooth, on-screen equation editor allowing the user to change the polynomial degree dynamically without recompiling.

## Interactive Controls
* **Change Polynomial Power ($n$):** Hover your mouse cursor over the exponent value $n$ in the top-right equation panel ($z^n - 1 = 0$). When highlighted, **Left-Click** to enter edit mode, type a new integer value (supported range: $2 \le n \le 256$), and press **Enter** (or **Right-Click**) to apply. 
* **Save High-Resolution Capture:** Hover over the download icon in the top-right corner and **Left-Click** to trigger a high-resolution render. The image is saved as `Newton_Fractal_n.png` directly in your working directory.


## Technologies
* **C++20**
* **Intel ISPC** 1.28+ (Implicit SPMD Program Compiler)
* **SFML 3.0.2** (Windowing, user input, texture drawing, and file exporting)
* **CMake** 3.19+ (Build system utilizing `FetchContent` for dependencies)

## Compilation & Build Instructions

### Prerequisites
Ensure the following tools are installed and accessible in your system's `PATH`:
* **C++ Compiler:** MSVC (Windows), GCC 11+, or Clang 13+ (Linux) with C++20 support.
* **CMake:** Version 3.19 or newer.
* **ISPC Compiler:** Intel Implicit SPMD Program Compiler (version 1.28+).

### Linux
Install required dependencies for windowing and graphics (required by SFML). On Fedora, you can use:
```bash
sudo dnf install libXrandr-devel libXcursor-devel libXi-devel libX11-devel \
  mesa-libGL-devel freetype-devel systemd-devel libudev-devel pkgconfig git

```

Generate build files and compile:

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .

```

### Windows
Ensure you have your compiler environment correctly configured (e.g., using the "x64 Native Tools Command Prompt for VS").

Generate build files and compile:

```cmd
mkdir build
cd build
cmake ..
cmake --build . --config Release

```

### Running the Application
The executable and its required resources (e.g., fonts) are generated in the build directory. Run the application directly from there:

<<<<<<< HEAD
> Note: This project has been tested and is intended to be built on Linux.
=======
* **Linux:** `./Newton_Fractal`
* **Windows:** `.\Release\Newton_Fractal.exe`

>>>>>>> 6148e37 (Updated README and CMakeLists)

## Showcase
<p align="center">
  <img src="https://github.com/user-attachments/assets/fd6fdab3-8485-4c0a-b833-75bcd3770385" width="32%" alt="Newton_Fractal_3" />
  <img src="https://github.com/user-attachments/assets/527c95c5-d3ff-4581-867c-fa8fb1a4e20c" width="32%" alt="Newton_Fractal_8" />
  <img src="https://github.com/user-attachments/assets/c807f980-a4d9-4899-9889-c66f97d175f2" width="32%" alt="Newton_Fractal_128" />
</p>
<<<<<<< HEAD



=======
>>>>>>> 6148e37 (Updated README and CMakeLists)
