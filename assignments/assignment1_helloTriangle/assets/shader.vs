#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 ourColor;
uniform vec3 colorEffect = vec3(1.0,1.0,1.0);
uniform vec3 positionEffect = vec3(0.0,1.0,1.0);
void main()
{ 
	gl_Position = vec4(aPos+positionEffect,1.0);
	ourColor = aColor * colorEffect;

}