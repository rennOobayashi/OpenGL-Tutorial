#version 330 core
out vec4 FragColor;

in vec2 texcoords;

uniform sampler2D sprite;
uniform vec3 tex_color;

void main()
{
    FragColor = vec4(tex_color, 1.0) * texture(sprite, texcoords);
}