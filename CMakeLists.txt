cmake_minimum_required(VERSION 3.8.2)

project(pong C)

set(SOURCES
    src/audio/audio.c
    src/game.c
    src/gfx/gfx.c
    src/gfx/shader.c
    src/gfx/text.c
    src/gfx/window.c
    src/main.c
    src/util/util.c
)

find_package(OpenAL REQUIRED)
file(COPY res DESTINATION ${CMAKE_BINARY_DIR})
add_executable(pong ${SOURCES})

set_property(TARGET pong PROPERTY C_STANDARD 11)
set_property(TARGET pong PROPERTY C_STANDARD_REQUIRED on)

target_link_libraries(pong PRIVATE cglm glfw glad freetype ${OPENAL_LIBRARY})

if (UNIX AND NOT APPLE)
  target_link_libraries(pong PRIVATE alut)
endif()

include_directories(src)
target_include_directories(pong PRIVATE ${OPENAL_INCLUDE_DIR})
add_subdirectory(lib)

if(MSVC)
  target_compile_options(pong PRIVATE /W4 /WX)
else()
  target_compile_options(pong PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

if(APPLE)
  target_compile_options(pong PRIVATE -Wno-deprecated-declarations)
endif()
