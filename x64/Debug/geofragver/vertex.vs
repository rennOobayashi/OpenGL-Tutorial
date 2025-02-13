#version 330 core
layout (location = 0) in vec3 apos;
layout (location = 1) in vec3 anormal;
layout (location = 2) in vec2 atexcoords;

out vec3 frag_pos;
out vec3 normal;
out vec2 texcoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform bool reverse_normal;

void main()
{
    vec4 view_pos = view * model * vec4(apos, 1.0);

    frag_pos = view_pos.xyz;
    texcoords = atexcoords;
    normal = transpose(inverse(mat3(model))) * (reverse_normal ? -anormal : anormal);
    
    gl_Position = projection * view_pos; 
}