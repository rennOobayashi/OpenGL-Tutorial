#version 330 core
out vec4 frag_color;

in VS_OUT {
    vec3 frag_pos;
    vec3 normal;
    vec2 texcoords;
} fs_in;

uniform sampler2D texture1;
uniform vec3 light_pos;
uniform vec3 view_pos;
uniform bool clickb;

void main()
{
    vec3 color = texture(texture1, fs_in.texcoords).rgb;
    //ambient
    vec3 ambient = 0.05 * color;
    //diffuse
    vec3 light_dir = normalize(light_pos - fs_in.frag_pos);
    vec3 normal = normalize(fs_in.normal);
    float diff = max(dot(light_dir, normal), 0.0);
    vec3 diffuse = diff * color;
    //specular
    vec3 specular;
    float spec = 0.0;
    vec3 view_dir = normalize(view_pos -fs_in.frag_pos);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float gamma = 2.2;

    //bilnn phong
    if (clickb) {
        vec3 halfway_dir = normalize(light_dir + view_dir);
        spec = pow(max(dot(normal, halfway_dir), 0.0), 32.0);
    }
    //phong
    else {
        spec = pow(max(dot(view_dir, reflect_dir), 0.0), 8.0);
    }

    specular = vec3(0.3) * spec;

    frag_color = vec4(ambient + diffuse + specular, 1.0);
    frag_color.rgb = pow(frag_color.rgb, vec3(1.0/gamma));
}
