attribute vec3 position;
attribute vec3 normal;
attribute vec3 tangent;
attribute vec2 texCoords0;

uniform mat4 modelViewProjMatrix;

uniform vec3 lightPosition;		// Object-space
uniform vec3 cameraPosition;	// Object-space

varying vec2 texCoords;
varying vec3 lightDirection;
varying vec3 halfAngle;

void main()
{
	texCoords = texCoords0;
	gl_Position = modelViewProjMatrix * vec4(position, 1.0);
}
