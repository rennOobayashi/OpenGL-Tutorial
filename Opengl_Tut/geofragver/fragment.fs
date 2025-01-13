#version 330 core
out vec4 frag_color;

in VS_OUT {
    vec3 frag_pos;
    vec3 normal;
    vec2 texcoords;
} fs_in;

uniform sampler2D texture1;
uniform vec3 light_pos;
uniform vec3 light_color;
uniform vec3 view_pos;

vec3 phong(vec3 normal, vec3 frag_pos, vec3 light_pos, vec3 light_color);

void main()
{
    vec3 color = texture(texture1, fs_in.texcoords).rgb;
    vec3 lighting = phong(normalize(fs_in.normal), fs_in.frag_pos, light_pos, light_color);

    color *= lighting;
    color = pow(color, vec3(1.0/2.2));

    frag_color = vec4(color, 1.0);
}

vec3 phong(vec3 normal, vec3 frag_pos, vec3 light_pos, vec3 light_color) {
    //diffuse
    vec3 light_dir = normalize(light_pos - frag_pos);
    float diff = max(dot(light_dir, normal), 0.0);
    vec3 diffuse = diff * light_color;
    //specular
    vec3 view_dir = normalize(view_pos -fs_in.frag_pos);
    vec3 reflect_dir = reflect(-light_dir, normal);
    vec3 halfway_dir = normalize(light_dir + view_dir);
    float spec = pow(max(dot(normal, halfway_dir), 0.0), 64.0);
    vec3 specular = spec * light_color;
    float distance = length(light_pos - frag_pos);
    float max_distance = 1.5;
    float attenuation = 1.0 / (distance * distance);

    diffuse *= attenuation;
    specular *= attenuation;

    return diffuse + specular;
}