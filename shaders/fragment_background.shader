#version 310 es
precision mediump float;
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;

void main(void)
{
    FragColor = texture(texture1, TexCoord);
}