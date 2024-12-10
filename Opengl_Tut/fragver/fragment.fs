#version 330 core
out vec4 frag_color;

in vec3 normal;
in vec3 position;

uniform vec3 camera_pos;    
uniform samplerCube skybox;

void main()
{
    // view/camera 방향 벡터 i를 계산
    vec3 i = normalize(position - camera_pos);
    //반사 벡터 r 계산(reflect함수로)
    vec3 r = reflect(i, normalize(normal));
    frag_color = vec4(texture(skybox, r).rgb, 1.0);
}
