#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 texcoords;


uniform bool  chaos;
uniform bool  confuse;
uniform bool  shake;
uniform float time;

void main()
{
    vec2 texture = vertex.zw;
    gl_Position = vec4(vertex.xy, 0.0, 1.0);

    //chaosand confuse manipulate coorinates and move the scene
    //both should not be run at the same time.
    if (chaos) {
        float strength = 0.3;
        vec2 pos = vec2(texture.x + sin(time) * strength, texture.y + cos(time) * strength);
        texcoords = pos;
    }
    else if (confuse) {
        texcoords = vec2(1.0 - texture.x, 1.0 - texture.y);
    }
    else {
        texcoords = texture;
    }

    if (shake) {
        float strength = 0.0f;
        gl_Position.x += cos(time * 10) * strength;
        gl_Position.y += cos(time * 15) * strength;
    }
}