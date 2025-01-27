
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

vec2 parallax_mapping(vec2 texcoords, vec3 view_dir);

float height_scale;

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
    float height = texture(depth_map, texcoords).r;
    return texcoords - view_dir.xy * (height * height_scale);
}