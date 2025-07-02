#version 330 core
out vec4 FragColor;

in vec2 texcoords;

uniform sampler2D img;
uniform vec3 tex_color;

uniform vec3 light_positions[4];
uniform vec3 light_colors[4];
void main()
{
    FragColor = vec4(tex_color, 1.0) * texture(img, texcoords);
}