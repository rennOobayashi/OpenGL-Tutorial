#version 330 core
out vec4 frag_color;

in vec2 tex_coord;

uniform sampler2D texture1;

void main()
{
    if (gl_FragCoord.y < 360) {
        frag_color = texture(texture1, tex_coord) * vec4(1.0, 1.0, 1.0, 1.0);
    }
    else {
        frag_color = texture(texture1, tex_coord) * vec4(0.4, 0.0, 0.4, 1.0);
    }
}
