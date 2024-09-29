#version 330 core
out vec4 FragColor;
  
in vec4 ourColor;
in vec2 TexCoord;

uniform sampler2D texture0;
uniform sampler2D texture2;
uniform vec2 backLoc;

void main()
{
    FragColor = mix(texture(texture0,TexCoord+backLoc), texture(texture2,TexCoord+backLoc),0.2);
}