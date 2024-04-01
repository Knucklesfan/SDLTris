#version 310 es
precision mediump float;
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;
uniform vec4 sineinfo;
uniform vec2 texinfo;
void main(void)
{   
    vec2 texturecoordinates = vec2(TexCoords.x, -TexCoords.y);
    color = texture(image, texturecoordinates);
}  
