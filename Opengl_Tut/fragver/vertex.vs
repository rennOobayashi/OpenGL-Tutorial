#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;

out vec2 tex_coord;
out vec3 frag_pos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	frag_pos = vec3(model * vec4(a_pos, 1.0));
	normal = mat3(transpose(inverse(model))) * a_normal;

	gl_Position = projection * view * vec4(frag_pos, 1.0);
	tex_coord = vec2(a_tex_coord.x, a_tex_coord.y);
}