#version 330

in vec3 vertex_position;
in vec3 vertex_normal;
in vec3 vertex_tangent;
in vec3 vertex_binormal;
in vec2 vertex_uv;

uniform mat4 modelViewProjMatrix;
uniform mat4 modelViewMatrix;

uniform vec3 light_position; // World-space
uniform vec3 camera_position; // World-space

out vec2 uv;

out vec3 lightVector;
out vec3 halfAngle;

out vec3 testVector;

void main()
{
	gl_Position = modelViewProjMatrix * vec4(vertex_position, 1.0);
	
	uv = vertex_uv;
	
	vec4 mvPosition = modelViewMatrix * vec4(vertex_position, 1.0);
	
	vec3 lightVec = normalize(light_position - mvPosition.xyz);
	lightVector.x = dot(lightVec, vertex_tangent);
	lightVector.y = dot(lightVec, vertex_binormal);
	lightVector.z = dot(lightVec, vertex_normal);
	lightVector = normalize(lightVector);
	
	vec3 eyeVec = normalize(camera_position - mvPosition.xyz);
	vec3 eyeVector = normalize(eyeVec);
	eyeVector.x = dot(eyeVec, vertex_tangent);
	eyeVector.y = dot(eyeVec, vertex_binormal);
	eyeVector.z = dot(eyeVec, vertex_normal);
	eyeVector = normalize(eyeVector);
	
	vec3 halfVec = normalize(eyeVec + lightVec);
	
	halfAngle.x = dot(halfVec, vertex_tangent);
	halfAngle.y = dot(halfVec, vertex_binormal);
	halfAngle.z = dot(halfVec, vertex_normal);
	halfAngle = normalize(halfAngle);
	
	//testVector = eyeVector;
}
