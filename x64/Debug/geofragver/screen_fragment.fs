#version 330 core
out vec4 frag_color;

in vec2 texcoords;

uniform sampler2D screen_texture1;

void main()
{
    vec3 col = texture(screen_texture1, texcoords).rgb;
    float grayscale = 0.2126 * col.r + 0.7152 * col.g + 0.0722 * col.b;
    frag_color = vec4(vec3(grayscale), 1.0);
}
