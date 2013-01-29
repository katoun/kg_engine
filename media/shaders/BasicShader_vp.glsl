attribute vec3 position;
attribute vec3 normal;
attribute vec2 texCoords0;

uniform mat4 modelViewProjMatrix;

varying vec2 texCoords;

void main()
{
	texCoords = texCoords0;
	gl_Position = modelViewProjMatrix * vec4(position, 1.0);
}
