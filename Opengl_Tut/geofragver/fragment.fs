#version 330 core
out vec4 frag_color;

in vec2 texcoords;

uniform sampler2D texture1;

void main()
{
    frag_color = texture(texture1, texcoords);
}
