#version 330 core
out vec4 frag_color;

in vec2 tex_coord;

uniform sampler2D screen_texture;

void main()
{
    float average;

    frag_color = texture(screen_texture, tex_coord);
    //average = (frag_color.r + frag_color.g + frag_color.b) / 3.0;
    
    //인간의 눈은 녹색에 예민하고 파란색에 덜 예민하여 물리적으로 weighted채널을 사용하는 것이 좋습니다.
    average = 0.2126 * frag_color.r + 0.7152 * frag_color.g + 0.0722 * frag_color.b;
    frag_color = vec4(average, average, average, 1.0);
}
