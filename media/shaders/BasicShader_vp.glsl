attribute vec3 pos;
attribute vec3 normal;
attribute vec2 texCoords0;

uniform mat4 modelViewProjMat;

varying vec2 texCoords;

void main()
{
	texCoords = texCoords0;
	gl_Position = modelViewProjMat * pos;
}
