#version 300 es
in highp vec2 TexCoords;
out highp vec4 color;

uniform sampler2D image;
uniform highp vec4 spriteColor;

void main()
{    
    highp vec4 tempcolor = texture(image, vec2(TexCoords.x,-TexCoords.y));
    color = vec4(tempcolor.x*spriteColor.x,tempcolor.y*spriteColor.y,tempcolor.z*spriteColor.z,tempcolor.w*spriteColor.w);
}  
