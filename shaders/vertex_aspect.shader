 #version 300 es
layout (location = 0) in highp vec3 aPos;
layout (location = 1) in highp vec3 aColor;
layout (location = 2) in highp vec2 aTexCoord;

out highp vec3 ourColor;
out highp vec2 TexCoord;
uniform mat4 model;

void main()
{
    gl_Position = model*vec4(aPos, 1.0);
    ourColor = aColor;
    TexCoord = aTexCoord;
}

