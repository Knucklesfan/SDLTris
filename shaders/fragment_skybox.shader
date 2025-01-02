#version 300 es
out highp vec4 FragColor;

in highp vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);
}