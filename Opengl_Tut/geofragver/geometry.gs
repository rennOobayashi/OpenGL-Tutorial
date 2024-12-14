#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

void point_position(vec4 position);

void main()
{
    point_position(gl_in[0].gl_Position);
}

void point_position(vec4 position) {
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0); //bottom left
    EmitVertex();
    gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0); //bottom right
    EmitVertex();
    gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0); //top left
    EmitVertex();
    gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0); //top right
    EmitVertex();
    gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0); //top
    EmitVertex();

    EndPrimitive();
}