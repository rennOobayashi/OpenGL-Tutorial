#version 330 core
out vec4 frag_color;
layout (location = 0) out vec3 gposition;
layout (location = 1) out vec3 gnormal;
layout (location = 2) out vec4 gcolor_spec;

in vec3 frag_pos;
in vec3 normal;
in vec2 texcoords;

uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;
uniform samplerCube depth_map;

uniform vec3 light_pos[32];
uniform vec3 view_pos;

uniform float far_cube;

float shadow_calculation(vec3 frag_pos, vec3 light_pos);

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
    vec3 color = texture(diffuse_texture, texcoords).rgb;
    vec3 normal = normalize(normal);
    vec3 light_color = vec3(0.3);
    //ambient
    vec3 ambient = 0.3 * light_color;
    vec3 shadow_light = vec3(0.0);
    
    for (int i = 0; i < 32; i++) {
        //diffuse
        vec3 light_dir = normalize(light_pos[i] - frag_pos);
        float diff = max(dot(light_dir, normal), 0.0);
        vec3 diffuse = diff * light_color;
        //specular
        vec3 view_dir = normalize(view_pos - frag_pos);
        vec3 reflect_dir = reflect(-light_dir, normal);
        vec3 halfway_dir = normalize(light_dir + view_dir);
        float spec = pow(max(dot(normal, halfway_dir), 0.0), 64.0);
        vec3 specular = spec * light_color;
        
        float shadow = shadow_calculation(frag_pos, light_pos[i]);

        shadow_light += (1.0 - shadow) * (diffuse + specular);
    }

    vec3 lighting = (ambient + shadow_light) * color;

    frag_color = vec4(lighting, 1.0);

    gposition = frag_pos;
    gnormal = normalize(normal);
    gcolor_spec.rgb = texture(diffuse_texture, texcoords).rgb;
    gcolor_spec.a = texture(specular_texture, texcoords).r;
}

float shadow_calculation(vec3 frag_pos, vec3 light_pos) {
    vec3 frag_to_light = frag_pos - light_pos;
    float current_depth = length(frag_to_light);
    float bias = 0.05;
    float shadow = 0;
    float view_distance = length(view_pos - frag_pos);
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