#version 330

in vec3 vertex_position;
in vec2 vertex_uv;

out vec2 uv;

uniform mat4 modelViewProjMatrix;

void main()
{
	gl_Position = modelViewProjMatrix * vec4(vertex_position, 1.0);
	
	uv = vertex_uv;
}
