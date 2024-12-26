#version 330 core
layout (location = 0) in vec2 apos;
layout (location = 1) in vec3 acolor;
layout (location = 2) in vec2 aoffset;

out vec3 fcolor;

void main()
{
    vec2 pos = apos * (gl_InstanceID / 100.0);
    gl_Position = vec4(pos + aoffset, 0.0, 1.0);
    fcolor = acolor;
}