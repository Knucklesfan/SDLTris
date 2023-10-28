#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;
uniform vec2 texOffset;
uniform vec2 scale;

void main()
{
    TexCoords = (vertex.zw*scale)+texOffset;
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}
