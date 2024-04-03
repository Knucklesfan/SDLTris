
#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 Position;  
in vec2 TexCoord;

uniform samplerCube cubeMap;
uniform sampler2D normalMap;  
uniform float offset;


void main()
{
    vec3 normal = texture(normalMap, vec2(TexCoord.x*8.0f,TexCoord.y*8.0f+offset)).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);   
    // // ambient
    // float ambientStrength = 0.1;
    // vec3 ambient = ambientStrength * lightColor;
  	
    // // diffuse 
    // vec3 norm = normalize(normal);
    // vec3 lightDir = normalize(lightPos - Position);
    // float diff = max(dot(norm, lightDir), 0.0);
    // vec3 diffuse = diff * lightColor;
    vec3 I = normalize(Position);
    vec3 R = reflect(I, normalize(normal));
    FragColor = mix(vec4(texture(cubeMap, R).rgb, 1.0),vec4(0,0,0,0),Position.z/200.0);
    // vec3 result = (ambient + diffuse) * objectColor;
    // FragColor = vec4(result, 1.0);
} 