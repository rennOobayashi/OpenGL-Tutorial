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

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    vec3 direction;

    float constant;
    float linear;
    float quadratic;
    float cutoff_in;
    float cutoff_out;
};


uniform Material material;
uniform Light light;
uniform vec3 view_pos;

void main()
{
    float spec;
    float diff;
    float distance;
    float attenunation;
    float theta;
    float epsilon;
    float intensity;
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	vec3 result;
    vec3 norm; 
    vec3 light_dir;
    vec3 view_dir;
    vec3 reflect_dir;

    light_dir = normalize(light.position - frag_pos);

    theta = dot(light_dir, normalize(-light.direction));
    epsilon = light.cutoff_in - light.cutoff_out;
    intensity = clamp((theta - light.cutoff_out) / epsilon, 0.0, 1.0);

    distance = length(light.position - frag_pos);
    attenunation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient = light.ambient * vec3(texture(material.diffuse, tex_coord));

    norm = normalize(normal);

    diff = max(dot(norm, light_dir), 0.0);
    diffuse = (light.diffuse * diff * vec3(texture(material.diffuse, tex_coord))) * intensity;

    view_dir = normalize(view_pos - frag_pos);
    reflect_dir = reflect(-light_dir, norm);

    spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    specular = (light.specular * spec * vec3(texture(material.specular, tex_coord))) * intensity;

    ambient *= attenunation;
    diffuse *= attenunation;
    specular *= attenunation;
    
    result = ambient + diffuse + specular;

    frag_color = vec4(result, 1.0);
}