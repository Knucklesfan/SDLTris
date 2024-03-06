#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform vec4 col;
uniform vec4 bordercol;
uniform float border_width;
uniform float aspect;
void main()
{   
   float maxX = 1.0 - border_width;
   float minX = border_width;
   float maxY = maxX / aspect;
   float minY = minX / aspect;

   if (TexCoords.x < maxX && TexCoords.x > minX &&
       TexCoords.y < maxX && TexCoords.y > minY) {
     color = col;
   } else {
     color =  bordercol;
   }
}  
