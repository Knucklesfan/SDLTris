#version 300 es
layout (location = 0) in highp vec3 aPos;
layout (location = 2) in highp vec2 aTexCoord;

out highp vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	
    gl_Position = projection * view * model * vec4(aPos, 1.0);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}