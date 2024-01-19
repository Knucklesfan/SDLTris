#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
// texture sampler
uniform sampler2D texture1;
uniform float time;


// Animator settings

const vec2 spriteSize = vec2(315.0, 457.0);

const int columns = 8;

const float animationCycleTime = 60.0 / 142.0;

const int length = 48;

// Pre-computed values
const float animationDeltaTime = animationCycleTime / float(length);
const int[] animation = int[](0,1,2,3,4,5,6,7,8);
void main()
{
    int frame = animation[(int(floor(time*100)))%8];
    vec2 vectorspace = vec2(
                    (TexCoord.x / columns),
                    (TexCoord.y / columns)
    );
    vectorspace.x += int(time)%8;
    vectorspace.y += frame*100;

    FragColor = texture(texture1,vec2(vectorspace.x,TexCoord.y/columns+(0)));
} 


