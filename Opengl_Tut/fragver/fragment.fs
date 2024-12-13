#version 420 core
out vec4 frag_color;
layout (depth_greater) out float gl_FragDepth;

in vec2 tex_coord;

uniform sampler2D front_tex;
uniform sampler2D back_tex;

void main()
{
    frag_color = vec4(1.0);
    gl_FragDepth = gl_FragCoord.z + 0.1;
}
