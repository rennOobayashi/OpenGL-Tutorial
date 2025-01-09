#version 330 core
layout (location = 0) in vec3 apos;
layout (location = 1) in vec3 anormal;
layout (location = 2) in vec2 atexcoords;

out VS_OUT {
    vec3 frag_pos;
    vec3 normal;
    vec2 texcoords;
} vs_out;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    vs_out.frag_pos = apos;
    vs_out.normal = anormal;
    vs_out.texcoords = atexcoords;

    gl_Position = projection * view * vec4(apos, 1.0); 
}