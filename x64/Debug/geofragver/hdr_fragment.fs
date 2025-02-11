#version 330 core
out vec4 frag_color;

in vec2 texcoords;

uniform sampler2D hdr_buffer;
uniform sampler2D bloom_blur;
uniform float exposure;

void main()
{
    const float gamma = 2.2;
    vec3 hdr_color = texture(hdr_buffer, texcoords).rgb;
    vec3 bloom_color = texture(bloom_blur, texcoords).rgb;

    hdr_color += bloom_color;

    vec3 result = vec3(1.0) - exp(-hdr_color * exposure);

    result = pow(result, vec3(1.0 / gamma));

    frag_color = vec4(result, 1.0);
}