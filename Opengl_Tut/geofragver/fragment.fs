#version 330 core
layout (location = 0) out vec4 frag_color;
layout (location = 1) out vec4 bright_color;

in VS_OUT {
    vec3 frag_pos;
    vec3 normal;
    vec2 texcoords;
} fs_in;

struct Light {
    vec3 pos;
    vec3 color;
};

uniform Light lights[4];
uniform sampler2D diffuse_texture;
uniform vec3 view_pos;

void main()
{
    vec3 color = texture(diffuse_texture, fs_in.texcoords).rgb;
    vec3 normal = normalize(fs_in.normal);

    vec3 ambient = 0.0 * color;
    vec3 lighting = vec3(0.0);
    vec3 view_dir = normalize(view_pos - fs_in.frag_pos);
    vec3 result;
    float brightness;

    for (int i = 0; i < 4; i++) {
        vec3 light_dir = normalize(lights[i].pos - fs_in.frag_pos);
        float diff = max(dot(light_dir, normal), 0.0);
        vec3 diffuse = lights[i].color * diff * color;
        float distance = length(fs_in.frag_pos - lights[i].pos);

        diffuse *= 1.0 / (distance * distance);

        lighting += diffuse;
    }

    result = ambient + lighting;
    brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));

    if (brightness > 1.0) {
        bright_color = vec4(result, 1.0);
    }
    else {
        bright_color = vec4(0.0, 0.0, 0.0, 1.0);
    }
    
    frag_color = vec4(result, 1.0);
}