#version 330 core
out vec4 frag_color;

in vec3 normal;
in vec3 position;

uniform vec3 camera_pos;    
uniform samplerCube skybox;

void main()
{
    if (gl_FragCoord.x < 640) {
        frag_color = vec4(1.0, 0.0, 0.0, 1.0);
    }
    else {
        frag_color = vec4(0.0, 0.0, 1.0, 1.0);
    }
}
