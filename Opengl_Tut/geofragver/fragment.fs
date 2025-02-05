#version 330 core
layout (location = 0) out vec3 gposition;
layout (location = 1) out vec3 gnormal;
layout (location = 2) out vec4 gcolor_spec;

in vec3 frag_pos;
in vec3 normal;
in vec2 texcoords;

uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;

void main()
{
    gposition = frag_pos;
    gnormal = normalize(normal);
    gcolor_spec.rgb = texture(diffuse_texture, texcoords).rgb;
    gcolor_spec.a = texture(specular_texture, texcoords).r;
}