#version 330 core
out vec4 frag_color;

in vec2 texcoords;

uniform sampler2D hdr_buffer;
uniform float exposure;

void main()
{
    const float gamma = 2.2;
    vec3 hdr_color = texture(hdr_buffer, texcoords).rgb;
    vec3 result = vec3(1.0) - exp(-hdr_color * exposure);

    result = pow(result, vec3(1.0 / gamma));

    frag_color = vec4(result, 1.0);
}