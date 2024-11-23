#version 330 core
out vec4 frag_color;

in vec3 our_color;    
in vec2 tex_coord;

uniform sampler2D texture1;
uniform sampler2D texture2;

float near = 0.1;
float far = 100.0;

float linearize_depth(float depth);

void main()
{
    //보여주기 위해 far로 나눔
    float depth = linearize_depth(gl_FragCoord.z) / far;
    frag_color = vec4(vec3(depth), 1.0);
}

float linearize_depth(float depth) {
    //depth value를 NDC 좌표로 변환
    float z = depth * 2.0 - 1.0;
    //z에 역변환을 적용시켜 선형 깊이 값 얻음
    return (2.0 * near * far) / (far + near - z * (far - near));
}