#version 330 core
out vec4 FragColor;

in vec2 texcoords;
in vec4 particle_color;

uniform sampler2D sprite;

void main()
{
    FragColor = (texture(sprite, texcoords) * particle_color);
}