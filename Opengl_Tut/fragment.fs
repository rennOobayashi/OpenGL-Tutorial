#version 330 core
out vec4 frag_color;
    
in vec2 tex_coord;
in vec3 frag_pos;
in vec3 normal;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 object_color;
uniform vec3 light_color;
uniform vec3 light_pos;

void main()
{
	float ambient_strength = 0.1;
    float diff;
	vec3 ambient;
	vec3 result;
    vec3 norm; 
    vec3 light_dir;
    vec3 diffuse;

    ambient = ambient_strength * light_color;

    //방향만 신경쓰기에 벡터 단위로 수행되는데, 벡터 정규화로 벡터 단위 인지 확인
    norm = normalize(normal);
    light_dir = normalize(light_pos - frag_pos);

    //norm과 light_dir 내적 (최종적으로 두 벡터 사이의 각이 클수록 duffuse 요소는 어두워짐, 각이 90도가 넘으면 결과가 음수가 되므로 max함수 사용해 두 파라미터중 더 큰 값 리턴)
    diff = max(dot(norm, light_dir), 0.0);
    //diffuse 효과 계산
    diffuse = diff * light_color;
    
    result = (ambient + diffuse) * object_color;

    frag_color = mix(texture(texture1, tex_coord), texture(texture2, tex_coord), 0.1) * vec4(result, 1.0);
}