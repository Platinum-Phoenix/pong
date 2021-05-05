#version 330 core
layout (location = 0) in vec2 position;

uniform mat3 model;
uniform mat4 view; 
uniform mat4 projection; 

void main() {
    vec3 world = model * vec3(position, 1.0);
    gl_Position = projection * view * vec4(world, 1.0);
}

// vim:ft=glsl
