#version 330 core
layout (location = 0) in vec3 aPos;  //위치 변수는 attribute position 0를 가짐
layout (location = 1) in vec3 aColor; //컬러 변수는 attribute position 1를 가짐
out vec3 ourColor;  //컬러를 fragment shader로 출력

void main()
{
	gl_Position = vec4(aPos, 1.0);
	ourColor = aColor; //vertex data로부터 가져와 컬러 입력을 ourColor에 설정
}