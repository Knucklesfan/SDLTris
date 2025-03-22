
#version 330 core
out vec4 FragColor;

in vec2 vUV;

uniform float time;
uniform sampler2D texture1;

float fullwidth = 7040.0;

void main()
{
    int uvy = int(vUV.y+mod((time/16),fullwidth));
      //vec3 backdrop = mix(C, C, C);
    FragColor = texture(texture1, vec2(vUV.x,uvy));

}
