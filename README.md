# pong

## About

A simple pong implementation using OpenGL 3.3 and C

## Tech Used

-   The C programming language
-   Open GL 3.3
-   OpenAL
-   [GLFW](https://glfw.org)
-   [glad2](https://github.com/Dav1dde/glad/tree/glad2)
-   [cglm](https://github.com/recp/cglm)
-   freetype 2

## Building

1.  Get your hands on CMake and a C compiler
2.  Initialize all the git submodules:
    ```sh
    git submodule init
    ```
3.  Create and move into a build directory: e.g.

    ```sh
    mkdir build && cd build
    ```
4.  Generate and build. For example, using the "Unix Makefile" generator:
    ```sh
    cmake ..
    make
    ```
    NOTE: make sure to regenerate (`cmake ..`) when you make changes to a shader

Let me know if you face any build issues :)
