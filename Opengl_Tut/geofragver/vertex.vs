#version 330 core
layout (location = 0) in vec3 apos;
layout (location = 1) in vec3 anormal;
layout (location = 2) in vec2 atexcoords;
layout (location = 3) in vec3 atangent;
layout (location = 4) in vec3 abitangent;

out vec2 texcoords;

out VS_OUT {
    vec3 frag_pos;
    vec2 texcoords;
    vec3 tangent_light_pos;
    vec3 tangent_view_pos;
    vec3 tangent_frag_pos;
} vs_out;

uniform vec3 light_pos;
uniform vec3 view_pos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    vs_out.frag_pos = vec3(model * vec4(apos, 1.0));
    vs_out.texcoords = atexcoords;
    gl_Position = projection * view * model * vec4(apos, 1.0);
    
    mat3 normal_matrix = transpose(inverse(mat3(model)));
    vec3 t = normalize(normal_matrix * atangent);
    vec3 n = normalize(normal_matrix * anormal);
    t = normalize(t - dot(t, n) * n);
    vec3 b = cross(n, t);
    mat3 tbn = transpose(mat3(t, b, n));
    vs_out.tangent_light_pos = tbn * light_pos;
    vs_out.tangent_view_pos = tbn * view_pos;
    vs_out.tangent_frag_pos = tbn * vs_out.frag_pos;
}