attribute vec3 position;
attribute vec3 normal;
attribute vec3 tangent;
attribute vec3 binormal;
attribute vec2 texCoords0;

uniform mat4 modelViewProjMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 inverseViewMatrix;

uniform vec3 lightPosition; // World-space
uniform vec3 cameraPosition; // World-space

varying vec2 texCoords;

varying vec3 lightVector;
varying vec3 halfAngle;

void main()
{
	texCoords = texCoords0;
	gl_Position = modelViewProjMatrix * vec4(position, 1.0);
	
	vec4 eyePosition = modelViewMatrix * vec4(position, 1.0);
	
	//**--------------------------------------------
	//** Calculate the light vector in object space,
	//** and then transform it into texture space.
	//**--------------------------------------------
	vec3 temp_light_position = vec3(vec4(lightPosition.x, lightPosition.y, -lightPosition.z, 1.0) * inverseViewMatrix);
	vec3 temp_light_vector   = temp_light_position.xyz - position.xyz;
	lightVector.x = dot(temp_light_vector, tangent);
	lightVector.y = dot(temp_light_vector, binormal);
	lightVector.z = dot(temp_light_vector, normal);

	//**-------------------------------------------
	//** Calculate the view vector in object space,
	//** and then transform it into texture space.
	//**-------------------------------------------
	vec4 oglEyePos = eyePosition;
	oglEyePos.z    = -oglEyePos.z;
	vec3 temp_eye_position = vec3(oglEyePos * inverseViewMatrix);
	vec3 temp_view_vector  = temp_eye_position - position.xyz;
	vec3 temp_view_vector2;
	temp_view_vector2.x = dot(temp_view_vector, tangent);
	temp_view_vector2.y = dot(temp_view_vector, binormal);
	temp_view_vector2.z = dot(temp_view_vector, normal);
	
	//**-------------------------
	//** Calculate the half angle
	//**-------------------------
	halfAngle = lightVector + temp_view_vector2;

	/////////////////////////////////
	
	/*vec3 lightVec = normalize(lightPosition - gl_Position.xyz);

	lightVector.x = dot(lightVec, tangent);
	lightVector.y = dot(lightVec, binormal);
	lightVector.z = dot(lightVec, normal);
	lightVector = normalize(lightVector);
	
	vec3 eyeVec = normalize(cameraPosition - gl_Position.xyz);
	vec3 halfVec = normalize(eyeVec + lightVec);
	
	halfAngle.x = dot(halfVec, tangent);
	halfAngle.y = dot(halfVec, binormal);
	halfAngle.z = dot(halfVec, normal);
	halfAngle = normalize(halfAngle);*/
}
