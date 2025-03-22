#version 330 core
layout (location = 0) in vec3 vertex; // <vec2 position, vec2 texCoords>
layout (location = 2) in vec2 aTexCoord;

out vec2 vUV;

void main() {
    gl_Position = vec4(vertex, 1.0);
    vUV = aTexCoord;
}