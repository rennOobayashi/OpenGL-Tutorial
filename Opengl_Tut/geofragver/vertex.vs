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

void main()
{
    frag_pos = vec3(model * vec4(apos, 1.0));
    normal = transpose(inverse(mat3(model))) * anormal;
    texcoords = atexcoords;
    gl_Position = projection * view * model * vec4(apos, 1.0); 
}