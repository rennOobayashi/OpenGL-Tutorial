#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT {
    vec3 color;
} gs_in[];

out vec3 fcolor;

void point_position(vec4 position);

void main()
{
    point_position(gl_in[0].gl_Position);
}

void point_position(vec4 position) {
    fcolor = vec3(1.0, 1.0, 1.0);
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0); //bottom left
    EmitVertex();
    gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0); //bottom right
    EmitVertex();
    
    //오직 하나의 입력 vertex만 존재
    fcolor = gs_in[0].color;
    gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0); //top left
    EmitVertex();
    gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0); //top right
    EmitVertex();
    gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0); //top
    EmitVertex();

    EndPrimitive();
}