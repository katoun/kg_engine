#version 330

in vec2 uv;

out vec4 fragColor;

uniform sampler2D diffuseMap;

void main()
{
	fragColor = texture2D(diffuseMap, uv);
}