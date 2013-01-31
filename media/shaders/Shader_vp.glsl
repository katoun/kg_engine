attribute vec3 position;
attribute vec3 normal;
attribute vec3 tangent;
attribute vec3 binormal;
attribute vec2 texCoords0;

uniform mat4 modelViewProjMatrix;
uniform mat4 modelViewMatrix;

uniform vec3 lightPosition; // World-space
uniform vec3 cameraPosition; // World-space

uniform vec3 lightPositionOS; // Object-space
uniform vec3 cameraPositionOS; // Object-space

varying vec2 texCoords;

varying vec3 lightVector;
varying vec3 halfAngle;

varying vec3 testVector;

void main()
{
	texCoords = texCoords0;
	gl_Position = modelViewProjMatrix * vec4(position, 1.0);
	
	vec4 mvPosition = modelViewMatrix * vec4(position, 1.0);
	
	vec3 lightVec = normalize(lightPosition - mvPosition.xyz);
	//vec3 lightVec = normalize(lightPositionOS);

	lightVector.x = dot(lightVec, tangent);
	lightVector.y = dot(lightVec, binormal);
	lightVector.z = dot(lightVec, normal);
	lightVector = normalize(lightVector);
	
	vec3 eyeVec = normalize(cameraPosition - mvPosition.xyz);
	//vec3 eyeVec = normalize(cameraPositionOS - position);
	vec3 eyeVector = normalize(eyeVec);
	eyeVector.x = dot(eyeVec, tangent);
	eyeVector.y = dot(eyeVec, binormal);
	eyeVector.z = dot(eyeVec, normal);
	eyeVector = normalize(eyeVector);
	
	vec3 halfVec = normalize(eyeVec + lightVec);
	
	halfAngle.x = dot(halfVec, tangent);
	halfAngle.y = dot(halfVec, binormal);
	halfAngle.z = dot(halfVec, normal);
	halfAngle = normalize(halfAngle);
	
	//testVector = eyeVector;
}
