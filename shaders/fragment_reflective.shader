
#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform samplerCube skybox;

void main()
{             
    vec3 I = normalize(Position - 0);
    vec3 R = reflect(I, normalize(Normal));
    FragColor = vec4(1,0,0,0);
}