#version 330 core
out vec4 FragColor;

in vec3 world_pos;

uniform samplerCube envronment_map;

void main()
{
    vec3 env_color = texture(envronment_map, world_pos).rgb;

    env_color /= (env_color + vec3(1.0));
    env_color = pow(env_color, vec3(1.0 / 2.2));

    FragColor = vec4(env_color, 1.0);
}