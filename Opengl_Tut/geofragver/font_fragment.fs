#version 330 core
out vec4 color;
in vec2 texcoords;

uniform sampler2D text;
uniform vec3 text_color;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, texcoords).r);
    color = vec4(text_color, 1.0) * sampled;
}