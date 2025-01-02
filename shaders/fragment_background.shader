#version 300 es
out highp vec4 FragColor;

in highp vec3 ourColor;
in highp vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, TexCoord);
}