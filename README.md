# pong

## About

A simple pong implementation using OpenGL 3.3 and C

## Tech Used

-   The C programming language
-   Open GL 3.3
-   [GLFW](https://glfw.org)
-   [glad2](https://github.com/Dav1dde/glad/tree/glad2)
-   [cglm](https://github.com/recp/cglm)

## Building

1.  Get your hands on CMake and a C compiler
2.  Create and move into a build directory: e.g.

    ```sh
    mkdir build && cd build
    ```

3.  Generate and build. For example, using the "Unix Makefile" generator:
    ```sh
    cmake ..
    make
    ```
    NOTE: make sure to regenerate (`cmake ..`) when you make changes to a shader
