#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform float loopHeight;
void main()
{    

    color = texture(image, vec2(TexCoords.x+0, -mod(TexCoords.y,loopHeight)));
}  
