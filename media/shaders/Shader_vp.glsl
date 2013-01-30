attribute vec3 position;
attribute vec3 normal;
attribute vec3 tangent;
attribute vec3 binormal;
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
	
	vec3 lightVec = normalize(lightPosition - gl_Position);

	lightDirection.x = dot(lightVec, tangent);
	lightDirection.y = dot(lightVec, binormal);
	lightDirection.z = dot(lightVec, normal);
	lightDirection = normalize(lightDirection);

	vec3 vertexPosition = normalize(gl_Position);
	vec3 halfVector = normalize(vertexPosition + lightVec);
	
	halfAngle.x = dot(halfVector, tangent);
	halfAngle.y = dot(halfVector, binormal);
	halfAngle.z = dot(halfVector, normal);
}
