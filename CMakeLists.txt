cmake_minimum_required(VERSION 3.8.2)

project(pong C)

set(SOURCES
	src/main.c
  src/window.c
)

add_executable(pong ${SOURCES})
target_link_libraries(pong PRIVATE cglm glfw glad)

set_property(TARGET pong PROPERTY C_STANDARD 11)
set_property(TARGET pong PROPERTY C_STANDARD_REQUIRED on)

add_subdirectory(lib/glfw)
add_subdirectory(lib/glad)
add_subdirectory(lib/cglm)
set(CGLM_STATIC ON)

if(MSVC)
  target_compile_options(pong PRIVATE /W4 /WX)
else()
  target_compile_options(pong PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

