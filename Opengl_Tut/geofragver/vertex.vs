#version 330 core
layout (location = 0) in vec3 apos;
layout (location = 1) in vec3 acolor;
layout (location = 2) in vec2 atex_coord;

out VS_OUT {
    vec2 tex_coords;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(apos, 1.0);
    vs_out.tex_coords = atex_coord;    
}