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
uniform sampler2D depth_map;

uniform float height_scale;

vec2 parallax_mapping(vec2 texcoords, vec3 view_dir);


void main()
{
    vec3 view_dir = normalize(fs_in.tangent_view_pos - fs_in.tangent_frag_pos);
    vec2 texcoords = parallax_mapping(fs_in.texcoords, view_dir);
    
    if (texcoords.x > 1.0 || texcoords.y > 1.0 || texcoords.x < 0.0 || texcoords.y < 0.0) {
        discard;
    }

    vec3 color = texture(diffuse_texture, texcoords).rgb;
    vec3 normal = texture(normal_map_texture, texcoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    //ambient
    vec3 ambient = 0.1 * color;

    //diffuse
    vec3 light_dir = normalize(fs_in.tangent_light_pos - fs_in.tangent_frag_pos);
    float diff = max(dot(light_dir, normal), 0.0);
    vec3 diffuse = diff * color;

    //specular
    vec3 reflect_dir = reflect(-light_dir, normal);
    vec3 halfway_dir = normalize(light_dir + view_dir);
    float spec = pow(max(dot(normal, halfway_dir), 0.0), 32.0);
    vec3 specular = spec * vec3(0.2);

    frag_color = vec4(ambient + diffuse + specular, 1.0);
}

vec2 parallax_mapping(vec2 texcoords, vec3 view_dir) {
    const float min_layers = 8.0, max_layers = 32.0;
    float num_layers = mix(max_layers, min_layers, max(dot(vec3(0.0, 0.0, 1.0), view_dir), 0.0));
    float layer_depth = 1.0 / num_layers;
    float current_layer_depth = 0.0;
    vec2 p = view_dir.xy * height_scale;
    vec2 delta_texcoords = p / num_layers;
    
    vec2 current_texcoords = texcoords;
    float current_depth_map_value = texture(depth_map, current_texcoords).r;

    while (current_layer_depth < current_depth_map_value) {
        current_texcoords -= delta_texcoords;
        current_depth_map_value = texture(depth_map, current_texcoords).r;
        current_layer_depth += layer_depth;
    }

    vec2 pre_texcoords = current_texcoords + delta_texcoords;
    float after_depth = current_depth_map_value - current_layer_depth;
    float before_depth = texture(depth_map, pre_texcoords).r - current_layer_depth + layer_depth;

    float weight = after_depth / (after_depth - before_depth);
    vec2 final_texcoords = pre_texcoords * weight + current_texcoords * (1.0 - weight);

    return final_texcoords;
}