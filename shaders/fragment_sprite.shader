#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;
uniform vec4 sineinfo;
uniform vec2 texinfo;
void main()
{    
    int width = 1;
    int height = 1; //make this a vec2
    int scanlinesize = 64;

    float scanline = floor((TexCoords.y*(texinfo.y))/sineinfo.z)*sineinfo.z;
    float sinex = (sin((sineinfo.w+scanline)*sineinfo.y/2)*sineinfo.x/texinfo.x);
    color = texture(image, vec2(TexCoords.x+0, -TexCoords.y));
}  
