#version 300 es
layout (location = 0) in highp vec4 vertex; // <vec2 position, vec2 texCoords>

out highp vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;
uniform highp vec2 texOffset;
uniform highp vec2 scale;

void main()
{
    TexCoords = (vertex.zw*scale)+texOffset;
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}
