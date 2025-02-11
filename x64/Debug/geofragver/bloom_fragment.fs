#version 330 core
layout (location = 0) out vec4 frag_color;
layout (location = 1) out vec4 bright_color;

in VS_OUT {
    vec3 frag_pos;
    vec3 normal;
    vec2 texcoords;
} fs_in;

uniform vec3 light_color;

void main()
{
    float brightness = 0.0;
    
    frag_color = vec4(light_color, 1.0);
    brightness = dot(frag_color.rgb, vec3(0.2126, 0.7152, 0.0722));

    if (brightness > 1.0) {
        bright_color = vec4(frag_color.rgb, 1.0);
    }
    else {
        bright_color = vec4(0.0, 0.0, 0.0, 1.0);
    }
}