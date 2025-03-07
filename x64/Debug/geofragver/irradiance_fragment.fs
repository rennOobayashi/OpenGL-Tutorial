#version 330 core
out vec4 FragColor;

in vec3 local_pos;

uniform samplerCube envronment_map;

const float pi = 3.14159265359;

void main()
{
    vec3 normal = normalize(local_pos);
    vec3 irradiance = vec3(0.0);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, normal));
    float sample_delta = 0.025;
    float nr_samples = 0.0;

    up = normalize(cross(normal, right));

    for (float phi = 0.0; phi < 2.0 * pi; phi += sample_delta) {
        for (float theta = 0.0; theta < 0.5 * pi; theta += sample_delta) {
            vec3 tangent_sample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            vec3 sample_vec = tangent_sample.x * right + tangent_sample.y * up + tangent_sample.z * normal;

            irradiance += texture(envronment_map, sample_vec).rgb * cos(theta) * sin(theta);
            nr_samples++;
        }
    }

    irradiance = pi * irradiance * (1.0 / float(nr_samples));

    FragColor = vec4(irradiance, 1.0);
}