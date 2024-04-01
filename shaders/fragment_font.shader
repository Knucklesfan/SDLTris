#version 310 es
precision mediump float;
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;

void main(void)
{    
    vec2 texturecoordinates = vec2(TexCoords.x, -TexCoords.y);

    vec4 tempcolor = texture(image, texturecoordinates);
    color = vec4(tempcolor.x*spriteColor.x,tempcolor.y*spriteColor.y,tempcolor.z*spriteColor.z,tempcolor.w);
}  
