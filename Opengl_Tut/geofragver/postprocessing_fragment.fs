#version 330 core
out vec4 FragColor;

in vec2 texcoords;
in bool ch;
in bool con;
in bool sh;

uniform sampler2D scene;
uniform vec2      offsets[9];
uniform int       edge_kernel[9];
uniform float     blur_kernel[9];

//Offsets, edge_kernel and blur_kernel only need to be set once, each time the fragment shader is executed.
void main()
{
    FragColor = vec4(0.0);
    vec3 sample[9];

    if (ch || sh) {
        for (int i = 0; i < 9; ++i) {
            sample[i] += vec3(texture(scene, texcoords.st + offsets[i]))
        }
    }

    if (ch) {
        for (int i = 0; i < 9; ++i) {
            FragColor += vec4(sample[i] * edge_kernel[i], 0.0);
        }

        FragColor.a = 1.0f;
    }
    else if (con) {
        FragColor = vec4(1.0 - texture(scene, texcoords).rgb, 1.0);
    }
    else if (sh) {
        for (int i = 0; i < 9; ++i) {
            FragColor += vec4(sample[i] * blur_kernel[i], 0.0);
        }
        FragColor.a = 1.0;
    }
    else {
        FragColor = texture(scene, texcoords);
    }
}