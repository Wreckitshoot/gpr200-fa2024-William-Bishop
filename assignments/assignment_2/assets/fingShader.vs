#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 ourColor;
out vec2 TexCoord;

uniform vec3 fingLoc;

void main()
{
    gl_Position = vec4(aPos + fingLoc, 1.0);
    ourColor = aColor;
    TexCoord = aTexCoord;
}