#version 330 core
out vec4 frag_color;

in vec2 texcoords;

struct light {
    vec3 position;
    vec3 color;
    float linear;
    float quadratic;
};

const int nr_lights = 32;

uniform sampler2D gposition;
uniform sampler2D gnormal;
uniform sampler2D gcolor_spec;

uniform light lights[nr_lights];
uniform vec3 view_pos;

void main()
{
    vec3 frag_pos = texture(gposition, texcoords).rgb;
    vec3 normal = texture(gnormal, texcoords).rgb;
    vec3 diffuse = texture(gcolor_spec, texcoords).rgb;
    float specular = texture(gcolor_spec, texcoords).a;

    vec3 lighting = diffuse * 0.1;
    vec3 view_dir = normalize(view_pos - frag_pos);

    for (int i = 0; i < nr_lights; i++) {
        //diffuse
        vec3 light_dir = normalize(lights[i].position - frag_pos);
        vec3 _diffuse = max(dot(normal, light_dir), 0.0) * diffuse * lights[i].color;
        //specular
        vec3 halfway_dir = normalize(light_dir + view_dir);
        float spec = pow(max(dot(normal, halfway_dir), 0.0), 16.0);
        vec3 _specular = lights[i].color * spec * specular;
        //attenuation
        float distance = length(lights[i].position - frag_pos);
        float attenuation = 1.0 / (1.0 + lights[i].linear * distance + lights[i].quadratic * distance * distance);
        _diffuse *= attenuation;
        _specular *= attenuation;
        lighting += _diffuse + _specular;
    }

    frag_color = vec4(lighting, 1.0);
}