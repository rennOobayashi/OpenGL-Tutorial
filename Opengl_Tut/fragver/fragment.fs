#version 330 core
out vec4 frag_color;

in vec3 normal;
in vec3 position;

uniform vec3 camera_pos;    
uniform samplerCube skybox;

void main()
{
    // light/view 광선이 공기에서 유리로 향하므로 air/glass
    float ratio = 1.00 / 1.52;
    // view/camera 방향 벡터 i를 계산
    vec3 i = normalize(position - camera_pos);
    //굴절 벡터 r 계산(refract함수로)
    vec3 r = refract(i, normalize(normal), ratio);
    frag_color = vec4(texture(skybox, r).rgb, 1.0);
}
