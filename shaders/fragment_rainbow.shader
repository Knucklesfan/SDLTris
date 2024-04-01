#version 310 es
precision mediump float;

out vec4 color;

uniform vec4 col;
void main(void)
{   
  color = col;
}  
