#version 300 es
in highp vec2 TexCoords;
out highp vec4 color;

uniform sampler2D image;
void main()
{    
    color = texture(image, vec2(TexCoords.x, -TexCoords.y));
}  
