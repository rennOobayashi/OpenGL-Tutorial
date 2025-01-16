#version 330 core
out vec4 frag_color;

in vec2 texcoords;

uniform sampler2D depth_map;
uniform float near_cube;
uniform float far_cube;

float linearize_depth(float depth);

void main()
{
    float depth_value = texture(depth_map, texcoords).r;
    frag_color = vec4(vec3(linearize_depth(depth_value) / far_cube), 1.0); // orthographic
}

float linearize_depth(float depth) {
    float z = depth * 2.0 - 1.0;

    return (2.0 * near_cube * far_cube) / (far_cube + near_cube - z * (far_cube - near_cube));
}
