#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;

void main()
{    

    vec4 tempcolor = texture(image, vec2(TexCoords.x+0, -TexCoords.y));
    color = vec4(tempcolor.x*spriteColor.x,tempcolor.y*spriteColor.y,tempcolor.z*spriteColor.z,tempcolor.w);
}  
