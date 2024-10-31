#version 330 core
out vec4 FragColor;  
in vec3 ourColor;
in vec2 texCoord;
uniform sampler2D Texture1;
uniform sampler2D Texture2;
void main()
{
    FragColor = mix(texture(Texture1, texCoord), texture(Texture2, texCoord), 0.2);
};