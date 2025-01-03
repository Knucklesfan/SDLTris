#version 300 es
out highp vec4 FragColor;

in highp vec2 TexCoord;
// texture sampler
uniform sampler2D texture1; //texture to not be paletted
uniform sampler2D texture2; //texture to be paletted

uniform highp vec4 palette[8]; //palette array to use (8 colors)

void main()
{
    highp vec4 paletterender = texture(texture2, TexCoord); //render the texture to palette

    lowp vec4 matteColor = texture(texture1,TexCoord); //render the matte
    highp vec4 palColor = vec4(0,0,0,0); //set to the empty color
    if(paletterender.a > 0.0) { //if there is something to draw
        palColor = palette[int(paletterender.r * 255.0)]; //then draw it
    }
    FragColor = mix(palColor,matteColor,matteColor.a); //mix em like paint
}