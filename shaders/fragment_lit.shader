

#version 300 es
out highp vec4 FragColor;

in highp vec3 Normal;  
in highp vec3 FragPos;  
in highp vec2 TexCoord;

uniform highp vec3 lightPos; 
uniform highp vec3 lightColor;
uniform highp vec3 objectColor;
// texture sampler
uniform sampler2D texture1;
uniform highp float alpha;

void main()
{
    // ambient
    highp float ambientStrength = 0.2;
    highp vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    highp vec3 norm = normalize(Normal);
    highp vec3 lightDir = normalize(lightPos - FragPos);
    highp vec3 dir = normalize(vec3(lightPos.x,lightPos.y-5.0,lightPos.z) - lightPos);
    highp float theta = dot(lightDir, normalize(-dir));
    if(theta > 0.59779827917) {
    highp float diff = max(dot(norm, lightDir), 0.0);
    highp vec3 diffuse = diff * lightColor;
    
    highp vec3 result = (ambient + diffuse) * texture(texture1, TexCoord).xyz;
    FragColor = vec4(result, alpha);
    }
    else {
        highp vec3 result = (ambient) * texture(texture1, TexCoord).xyz;
        FragColor = vec4(result, alpha+0.05);
    }
} 

