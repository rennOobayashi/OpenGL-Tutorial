#version 330 core
layout (location = 0) out vec3 gposition;
layout (location = 1) out vec3 gnormal;
layout (location = 2) out vec4 gcolor_spec;

in VS_OUT {
    vec3 frag_pos;
    vec3 normal;
    vec2 texcoords;
} fs_in;

uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;

void main()
{
    gposition = fs_in.frag_pos;
    gnormal = normalize(fs_in.normal);
    gcolor_spec.rgb = texture(diffuse_texture, fs_in.texcoords).rgb;
    gcolor_spec.a = texture(specular_texture, fs_in.texcoords).r;
}