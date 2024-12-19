#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
// texture sampler
uniform sampler2D texture1;
uniform float time;

void main()
{
    FragColor = texture(texture1, vec2(TexCoord.x,-TexCoord.y/2+(time/100)))-TexCoord.y/1.5;
}
