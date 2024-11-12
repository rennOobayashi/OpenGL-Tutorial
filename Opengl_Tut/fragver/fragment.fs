#version 330 core
out vec4 frag_color;
    
in vec2 tex_coord;
in vec3 frag_pos;
in vec3 normal;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
};


uniform Material material;
uniform Light light;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 view_pos;

void main()
{
	float ambient_strength = 0.1;
    float specular_strength = 0.7;
    float spec;
    float diff;
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	vec3 result;
    vec3 norm; 
    vec3 light_dir;
    vec3 view_dir;
    vec3 reflect_dir;

    ambient = material.ambient * light.ambient;

    norm = normalize(normal);
    light_dir = normalize(light.position - frag_pos);

    diff = max(dot(norm, light_dir), 0.0);
    diffuse = (diff * material.diffuse) * light.diffuse;

    view_dir = normalize(view_pos - frag_pos);
    //light_dir는 현재 fragment에서 광원으로 향하는 방향이므로 -, 법선 벡터가 필요하므로 norm
    reflect_dir = reflect(-light_dir, norm);

    spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    specular = (material.specular * spec) * light.specular;
    
    result = ambient + diffuse + specular;

    frag_color = mix(texture(texture1, tex_coord), texture(texture2, tex_coord), 0.1) * vec4(result, 1.0);
}