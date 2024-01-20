#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
// texture sampler
uniform sampler2D texture1;
uniform float time;
uniform float parameter1;

// Animator settings

const vec2 spriteSize = vec2(315.0, 457.0);

const int columns = 8;

const float animationCycleTime = 60.0 / 142.0;

const int length = 48;

// Pre-computed values
const float animationDeltaTime = animationCycleTime / 60.0;

void main()
{
    int frame = (int(floor(time*100)))%length;
    vec2 vectorspace = vec2(
                    (TexCoord.x / 8.0)+(frame%columns)/8.0,
                    (TexCoord.y / 6.0)-(frame/columns)/6.0
    );

    FragColor = texture(texture1,vec2(vectorspace.x,vectorspace.y));
} 


