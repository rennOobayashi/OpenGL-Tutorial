#version 330 core
out vec4 frag_color;
    
in vec2 tex_coord;
in vec3 frag_pos;
in vec3 normal;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 object_color;
uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 view_pos;

void main()
{
	float ambient_strength = 0.1;
    float specular_strength = 0.7;
    float spec;
    float diff;
	vec3 ambient;
	vec3 result;
    vec3 norm; 
    vec3 light_dir;
    vec3 view_dir;
    vec3 reflect_dir;
    vec3 diffuse;
    vec3 specular;

    ambient = ambient_strength * light_color;

    norm = normalize(normal);
    light_dir = normalize(light_pos - frag_pos);

    diff = max(dot(norm, light_dir), 0.0);
    diffuse = diff * light_color;

    view_dir = normalize(view_pos - frag_pos);
    //light_dir는 현재 fragment에서 광원으로 향하는 방향이므로 -, 법선 벡터가 필요하므로 norm
    reflect_dir = reflect(-light_dir, norm);

    spec = pow(max(dot(view_dir, reflect_dir), 0.0), 16); //16 = shininess
    specular = specular_strength * spec * light_color;
    
    result = (ambient + diffuse + specular) * object_color;

    frag_color = mix(texture(texture1, tex_coord), texture(texture2, tex_coord), 0.1) * vec4(result, 1.0);
}