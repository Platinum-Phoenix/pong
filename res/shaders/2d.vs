#version 330 core
layout (location = 0) in vec3 posistion;

void main() {
    gl_Position = vec4(posistion, 1.0);
}

// vim:ft=glsl
