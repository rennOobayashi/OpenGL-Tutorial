#version 330 core
layout (location = 0) in vec3 apos;
layout (location = 1) in vec3 anormal;
layout (location = 2) in vec2 atexcoords;

out vec3 world_pos;
out vec3 normal;
out vec2 texcoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normal_matrix;

void main()
{
    texcoords = atexcoords;
    world_pos = vec3(model * vec4(apos, 1.0));
    normal = anormal * normal_matrix;

    gl_Position = projection * view * vec4(world_pos, 1.0);
}