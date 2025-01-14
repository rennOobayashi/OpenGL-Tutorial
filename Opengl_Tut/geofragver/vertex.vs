#version 330 core
layout (location = 0) in vec3 apos;
layout (location = 1) in vec3 anormal;
layout (location = 2) in vec2 atexcoords;

out vec2 texcoords;

out VS_OUT {
    vec3 frag_pos;
    vec3 normal;
    vec2 texcoords;
    vec4 frag_pos_light_space;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 light_space_matrix;

void main()
{
    vs_out.frag_pos = vec3(model * vec4(apos, 1.0));
    vs_out.normal = transpose(inverse(mat3(model))) * anormal;
    vs_out.texcoords = atexcoords;
    vs_out.frag_pos_light_space = light_space_matrix * vec4(vs_out.frag_pos, 1.0);
    gl_Position = projection * view * model * vec4(apos, 1.0); 
}