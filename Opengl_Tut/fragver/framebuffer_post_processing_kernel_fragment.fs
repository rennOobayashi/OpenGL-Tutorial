#version 330 core
out vec4 frag_color;

in vec3 our_color;    
in vec2 tex_coord;

uniform sampler2D screen_texture;

const float offset = 1.0 / 300.0;

void main()
{
    vec2 offsets[9] = vec2 [] (
        vec2(-offset,  offset),
        vec2( 0.0f,    offset),
        vec2( offset,  offset),
        vec2(-offset,  0.0f),
        vec2( 0.0f,    0.0f),
        vec2( offset,  0.0f),
        vec2(-offset, -offset),
        vec2( 0.0f,   -offset),
        vec2( offset, -offset)
    );
    float kernel[9] = float[] (
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    vec3 sample_tex[9];
    vec3 col = vec3(0.0);

    for (int i = 0; i < 9; i++) {
        sample_tex[i] = vec3(texture(screen_texture, tex_coord.st + offsets[i]));
        col += sample_tex[i] * kernel[i];
    }

    frag_color = vec4(col, 1.0);
}
