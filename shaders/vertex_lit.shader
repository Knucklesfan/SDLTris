#version 300 es
layout (location = 0) in highp vec3 aPos;
layout (location = 1) in highp vec3 aNormal;
layout (location = 2) in highp vec2 aTexCoord;

out highp vec3 FragPos;
out highp vec3 Normal;
out highp vec2 TexCoord;
//this is really basic, baby stuff really
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = aNormal;  
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
    gl_Position = projection * view * vec4(FragPos, 1.0);
}