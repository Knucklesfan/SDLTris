

#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoord;

uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
// texture sampler
uniform sampler2D texture1;
uniform float alpha;

void main()
{
    // ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 dir = normalize(vec3(lightPos.x,lightPos.y-5,lightPos.z) - lightPos);
    float theta = dot(lightDir, normalize(-dir));
    if(theta > 0.59779827917) {
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    vec3 result = (ambient + diffuse) * texture(texture1, TexCoord).xyz;
    FragColor = vec4(result, alpha);
    }
    else {
        vec3 result = (ambient) * texture(texture1, TexCoord).xyz;
        FragColor = vec4(result, alpha+0.05);
    }
} 

