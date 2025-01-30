#version 330 core
out vec4 frag_color;

in VS_OUT {
    vec3 frag_pos;
    vec3 normal;
    vec2 texcoords;
} fs_in;

struct Light {
    vec3 pos;
    vec3 color;
};

uniform Light lights[16];
uniform sampler2D diffuse_texture;
uniform vec3 view_pos;

void main()
{
    vec3 color = texture(diffuse_texture, fs_in.texcoords).rgb;
    vec3 normal = normalize(fs_in.normal);

    vec3 ambient = 0.0 * color;
    vec3 lighting = vec3(0.0);

    for (int i = 0; i < 16; i++) {
        vec3 light_dir = normalize(lights[i].pos - fs_in.frag_pos);
        float diff = max(dot(light_dir, normal), 0.0);
        vec3 diffuse = lights[i].color * diff * color;
        float distance = length(fs_in.frag_pos - lights[i].pos);

        diffuse *= 1.0 / (distance * distance);

        lighting += diffuse;
    }
    
    frag_color = vec4(ambient + lighting, 1.0);
}