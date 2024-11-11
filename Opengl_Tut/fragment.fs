#version 330 core
out vec4 FragColor;
in vec2 texCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 object_color;
uniform vec3 light_color;

void main()
{
	float ambient_strength = 0.1;
	vec3 ambient = ambient_strength * light_color;
	vec3 result = ambient * object_color;

    FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.1) * vec4(result, 1.0);
}