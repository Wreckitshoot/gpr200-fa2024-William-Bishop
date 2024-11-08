#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec2 TexCoord;
in vec3 FragPos; 

uniform float specularStrength;
uniform float diffuseStrength;
uniform float shininess;
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform vec3 ambience;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;


void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm,lightDir),diffuseStrength);
    vec3 diffuse = diff * lightColor;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    
    float spec = pow(max(dot(norm, halfwayDir), specularStrength), shininess);
    
    vec3 specular = spec * lightColor;  
    vec4 blockColor = vec4(0.5,0.7,0.1,1.0);
    //FragColor = blockColor * (ambience + vec4(diffuse,0.0) + vec4(specular,0.0));
    FragColor = texture(texture0,TexCoord) * vec4(ambience + diffuse + specular,1.0);
}