#version 330 core
out vec4 frag_color;

in VS_OUT {
    vec3 frag_pos;
    vec2 texcoords;
    vec3 tangent_light_pos;
    vec3 tangent_view_pos;
    vec3 tangent_frag_pos;
} fs_in;

uniform sampler2D diffuse_texture;
uniform sampler2D normal_map_texture;
uniform samplerCube depth_map;

uniform float far_cube;

float shadow_calculation(vec3 frag_pos);

vec3 sample_offset_directions[20] = vec3[]
(
    vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
    vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
    vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
    vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
    vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

void main()
{
    vec3 color = texture(diffuse_texture, fs_in.texcoords).rgb;
    vec3 normal = texture(normal_map_texture, fs_in.texcoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    vec3 light_color = vec3(0.3);
    //ambient
    vec3 ambient = 0.3 * light_color;
    //diffuse
    vec3 light_dir = normalize(fs_in.tangent_light_pos - fs_in.tangent_frag_pos);
    float diff = max(dot(light_dir, normal), 0.0);
    vec3 diffuse = diff * light_color;
    //specular
    vec3 view_dir = normalize(fs_in.tangent_view_pos - fs_in.tangent_frag_pos);
    vec3 reflect_dir = reflect(-light_dir, normal);
    vec3 halfway_dir = normalize(light_dir + view_dir);
    float spec = pow(max(dot(normal, halfway_dir), 0.0), 64.0);
    vec3 specular = spec * light_color;

    float shadow = shadow_calculation(fs_in.frag_pos);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

    frag_color = vec4(lighting, 1.0);
}

float shadow_calculation(vec3 frag_pos) {
    vec3 frag_to_light = frag_pos - fs_in.tangent_light_pos;
    float current_depth = length(frag_to_light);
    float bias = 0.05;
    float shadow = 0;
    float view_distance = length(fs_in.tangent_view_pos - frag_pos);
    //shadow LoD
    float disk_radius = (1.0 + (view_distance / far_cube)) / 25.0;
    int sample = 20;

    for (int i = 0; i < sample; i++) {
        float closeset_depth = texture(depth_map, frag_to_light + sample_offset_directions[i] * disk_radius).r;
        closeset_depth *= far_cube;

        if (current_depth - bias > closeset_depth) {
            shadow +=  1.0;
        }
    }

    shadow /= float(sample);

    return shadow;
}