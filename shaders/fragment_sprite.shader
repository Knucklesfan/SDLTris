#version 300 es
in highp vec2 TexCoords;
out highp vec4 color;

uniform sampler2D image;
uniform highp vec3 spriteColor;
uniform highp vec4 sineinfo;
uniform highp vec2 texinfo;
void main()
{    
    int width = 1;
    int height = 1; //make this a vec2
    int scanlinesize = 64;

    color = texture(image, vec2(TexCoords.x, -TexCoords.y));
}  
