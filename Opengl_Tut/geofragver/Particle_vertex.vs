#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 texcoords;
out vec4 particle_color;

uniform mat4 projection;
uniform vec2 offset;
uniform vec2 color;

void main()
{
    float scale = 10.0f;
    texcoords = vertex.zw;
    particle_color = color;
    gl_Position = projection * vec4((vertex.xy * scale) + offset, 0.0, 1.0);
}