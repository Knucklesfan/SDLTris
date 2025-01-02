#version 300 es
out highp vec4 FragColor;

in highp vec2 TexCoord;
in highp vec3 Normal;
in highp vec3 Position;

uniform highp vec3 cameraPos;
uniform sampler2D texture1;
uniform sampler2D reflectionmap;
uniform sampler2D reflectionmask;

void main()
{             
    highp float ratio = 1.00 / 1.52;
    highp vec3 I = normalize(Position-cameraPos);
    highp vec3 R = refract(I, normalize(Normal), ratio);
    FragColor = mix(
        texture(texture1,TexCoord),
        mix(
            texture(texture1,TexCoord),
            texture(reflectionmap,TexCoord),
            R.r),
        texture(reflectionmask,TexCoord).w);
}