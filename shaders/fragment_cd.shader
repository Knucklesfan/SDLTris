#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform sampler2D texture1;
uniform sampler2D reflectionmap;
uniform sampler2D reflectionmask;

void main()
{             
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(Position-cameraPos);
    vec3 R = refract(I, normalize(Normal), ratio);
    FragColor = mix(
        texture(texture1,TexCoord),
        mix(
            texture(texture1,TexCoord),
            texture(reflectionmap,TexCoord),
            R.r),
        texture(reflectionmask,TexCoord).w);
}