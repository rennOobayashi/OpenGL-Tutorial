#version 330 core
out vec4 frag_color;

in vec3 our_color;    
in vec2 tex_coord;

uniform sampler2D screen_texture;

void main()
{
    frag_color = vec4(vec3(1.0 - texture(screen_texture, tex_coord)), 1.0);
}