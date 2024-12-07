#version 330 core
out vec4 frag_color;

in vec3 our_color;    
in vec2 tex_coord;

uniform sampler2D texture1;

void main()
{
    frag_color = vec4(vec3(1.0 - texture(texture1, tex_coord)), 1.0);
}
