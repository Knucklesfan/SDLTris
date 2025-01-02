#version 300 es
in highp vec2 TexCoords;
out highp vec4 color;

uniform sampler2D text;
uniform highp vec3 textColor;

void main()
{    
    highp vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
}  