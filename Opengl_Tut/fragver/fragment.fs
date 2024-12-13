#version 330 core
out vec4 frag_color;

in vec2 tex_coord;

uniform sampler2D front_tex;
uniform sampler2D back_tex;

void main()
{
    if (gl_FrontFacing) {
        frag_color = texture(front_tex, tex_coord);
    }
    else {
        frag_color = texture(back_tex, tex_coord);
    }
}
