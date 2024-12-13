#version 330 core
out vec4 frag_color;

in VS_OUT
{
    vec2 tex_coord;
} fs_in;

uniform sampler2D texture1;

void main()
{
    frag_color = texture(texture1, fs_in.tex_coord);
}
