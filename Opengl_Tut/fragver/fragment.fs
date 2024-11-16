#version 330 core
out vec4 frag_color;
    
in vec2 tex_coord;
in vec3 frag_pos;
in vec3 normal;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Dir_Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
};

struct Point_Light {
    float constant;
    float linear;
    float quadraitc;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
};
#define NR_POINT_LIGHTS 4

vec3 calc_dir_light(Dir_Light light, vec3 normal, vec3 view_dir);
vec3 calc_point_light(Point_Light light, vec3 normal, vec3 frag_pos, vec3 view_dir);

uniform Material material;
uniform Dir_Light dir_light;
uniform Point_Light point_lights[NR_POINT_LIGHTS];
uniform vec3 view_pos;

void main()
{
    //attribute
	vec3 result;
    vec3 norm = normalize(normal); 
    vec3 view_dir = normalize(view_pos - frag_pos);

    //1. Directional lighting
    result = calc_dir_light(dir_light, norm, view_pos);

    //2. Point lighting
    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += calc_point_light(point_lights[i], norm, frag_pos, view_dir);
    }

    //3. spot lighting (if use spotlighting)
    //result += calc_spot_light(spot_light, norm, frag_pos, view_dir);

    frag_color = vec4(result, 1.0);
}

vec3 calc_dir_light(Dir_Light light, vec3 normal, vec3 view_dir) {
    vec3 light_dir = normalize(-light.direction);
    //diffuse
    float diff  = max(dot(normal, light_dir), 0.0);
    //specular
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    //결과 결합
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, tex_coord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, tex_coord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, tex_coord));

    return (ambient + diffuse + specular);
}

vec3 calc_point_light(Point_Light light, vec3 normal, vec3 frag_pos, vec3 view_dir) {
    vec3 light_dir = normalize(light.position - frag_pos);
    //diffuse
    float diff  = max(dot(normal, light_dir), 0.0);
    //specular
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    float distance = length(light.position - frag_pos);
    float attenunation = 1.0 / (light.constant + light.linear * distance + light.quadraitc * (distance * distance));

    //결과 결합
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, tex_coord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, tex_coord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, tex_coord));

    ambient *= attenunation;
    diffuse *= attenunation;
    specular *= attenunation;

    return (ambient + diffuse + specular);
}