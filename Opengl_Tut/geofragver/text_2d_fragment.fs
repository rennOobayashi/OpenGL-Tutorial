#version 330 core
out vec4 FragColor;

in vec2 texcoords;

uniform sampler2D text;

void main()
{
    vec4 sampled  = vec4(1.0 , 1.0, 1.0 texture(text, texcoords).r)
    FragColor = vec4(tex_color, 1.0) *sampled;
}