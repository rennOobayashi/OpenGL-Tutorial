#version 330 core
in vec4 frag_pos;

uniform vec3 light_pos;
uniform float far_cube;

void main()
{
    float light_distance = length(frag_pos.xyz - light_pos);

    light_distance = light_distance / far_cube;

    gl_FragDepth = light_distance;
}