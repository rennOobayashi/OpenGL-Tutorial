#version 330 core
out vec4 FragColor;
in vec3 ourColor; //Vertex Shader로 입력 받은 입력 변수 (이름과 타입이 같아야 함)

void main()
{
	FragColor = vec4(ourColor, 1.0);
}