cmake_minimum_required(VERSION 3.8.2)

project(pong C)

set(SOURCES
	src/main.c
  src/game.c
  src/gfx/gfx.c
  src/gfx/shader.c
  src/gfx/text.c
  src/gfx/window.c
  src/util/util.c
)

file(COPY res DESTINATION ${CMAKE_BINARY_DIR})
add_executable(pong ${SOURCES})

set_property(TARGET pong PROPERTY C_STANDARD 11)
set_property(TARGET pong PROPERTY C_STANDARD_REQUIRED on)

target_link_libraries(pong PRIVATE cglm glfw glad freetype)
include_directories(src)
add_subdirectory(lib)

if(MSVC)
  target_compile_options(pong PRIVATE /W4 /WX)
else()
  target_compile_options(pong PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
