#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 tex_coords;
} gs_in[];

out vec2 tex_coords;

uniform float time;

vec3 get_normal();
vec4 explode(vec4 position, vec3 normal);

void main()
{
    vec3 normal = get_normal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
    tex_coords = gs_in[0].tex_coords;
    EmitVertex();
    
    gl_Position = explode(gl_in[1].gl_Position, normal);
    tex_coords = gs_in[1].tex_coords;
    EmitVertex();

    gl_Position = explode(gl_in[2].gl_Position, normal);
    tex_coords = gs_in[2].tex_coords;
    EmitVertex();

    EndPrimitive();
}

vec3 get_normal() {
    vec3 x = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 y = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);

    return normalize(cross(x, y));
}

vec4 explode(vec4 position, vec3 normal) {
    float magnitude = 2.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
    
    return position + vec4(direction, 0.0);
}