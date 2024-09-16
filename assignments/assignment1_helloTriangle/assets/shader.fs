#version 330 core
//out vec4 FragColor;
//in vec3 ourColor;
uniform vec4 colorEffect;
void main()
{
	FragColor = colorEffect;
}