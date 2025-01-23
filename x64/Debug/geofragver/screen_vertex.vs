#version 330 core
layout (location = 0) in vec3 apos;
layout (location = 1) in vec2 atexcoords;

out vec2 texcoords;

void main()
{
    gl_Position = vec4(apos.x, apos.y, 1.0, 1.0); 
    texcoords = atexcoords;
}