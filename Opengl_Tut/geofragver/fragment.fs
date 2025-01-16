#version 330 core
out vec4 frag_color;

in VS_OUT {
    vec3 frag_pos;
    vec3 normal;
    vec2 texcoords;
    vec4 frag_pos_light_space;
} fs_in;

uniform sampler2D diffuse_texture;
uniform sampler2D shadow_map;

uniform vec3 light_pos;
uniform vec3 view_pos;

float shadow_calculation(vec4 frag_pos_light_space, vec3 normal, vec3 light_dir);

void main()
{
    vec3 color = texture(diffuse_texture, fs_in.texcoords).rgb;
    vec3 normal = normalize(fs_in.normal);
    vec3 light_color = vec3(0.3);
    //ambient
    vec3 ambient = 0.3 * light_color;
    //diffuse
    vec3 light_dir = normalize(light_pos - fs_in.frag_pos);
    float diff = max(dot(light_dir, normal), 0.0);
    vec3 diffuse = diff * light_color;
    //specular
    vec3 view_dir = normalize(view_pos - fs_in.frag_pos);
    vec3 reflect_dir = reflect(-light_dir, normal);
    vec3 halfway_dir = normalize(light_dir + view_dir);
    float spec = pow(max(dot(normal, halfway_dir), 0.0), 64.0);
    vec3 specular = spec * light_color;

    float shadow = shadow_calculation(fs_in.frag_pos_light_space, normal, light_dir);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

    frag_color = vec4(lighting, 1.0);
}

float shadow_calculation(vec4 frag_pos_light_space, vec3 normal, vec3 light_dir) {
    vec3 projcoords = frag_pos_light_space.xyz / frag_pos_light_space.w;
    projcoords = projcoords * 0.5 + 0.5;
    
    float closest_depth = texture(shadow_map, projcoords.xy).r;
    float current_depth = projcoords.z;
    float bias = max(0.05 * (1.0 - dot(normal, light_dir)), 0.005);
    float shadow = current_depth - bias > closest_depth ? 1.0 : 0.0;

    return shadow;
}