#version 300 es
out highp vec4 FragColor;

in highp vec2 TexCoord;
// texture sampler
uniform sampler2D texture1;
uniform lowp float time;
uniform lowp float combo;
lowp float rand(lowp vec2 coord) {
    return fract(sin(dot(coord,vec2(12.9898,78.233)))*43758.5453);
}
void main()
{
    FragColor = texture(texture1, TexCoord);
    if(combo > 0.0 && FragColor != vec4(1.0,1.0,1.0,1.0) && FragColor.a > 0.0) {
        highp float fireoffset = sin((TexCoord.y)*8.0+time*8.0)/(32.0-cos(time*16.0)*8.0);
        highp float otheroffset = cos((TexCoord.y)*4.0+time*16.0)/(48.0-sin(time*8.0)*8.0)-0.10;

        if(TexCoord.x < combo+otheroffset) {
            lowp float noise = rand(TexCoord+time)*0.2;
            FragColor = vec4(1.0,0.25+(TexCoord.x/(0.5+otheroffset))*0.25,0.0,1.0)+vec4(noise,noise,0,1.0);

        }
        else if(TexCoord.x < combo+fireoffset) {
            FragColor = vec4(1.0,1.0,0.0,1.0);
        }

    }
}
