#version 330

in vec3 vertex_position;
in vec3 vertex_normal;
in vec3 vertex_tangent;
in vec3 vertex_binormal;
in vec2 vertex_uv;

out vec2 uv;

out vec3 lightVector; // Tangent-space
out vec3 halfVector; // Tangent-space

out vec3 testVector;

uniform mat4 modelViewProjMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

uniform vec3 light_position; // World-space
uniform vec3 camera_position; // World-space

void main()
{
	gl_Position = modelViewProjMatrix * vec4(vertex_position, 1.0);
	
	uv = vertex_uv;
	
	// Building the matrix Camera Space -> Tangent Space
	vec3 n = normalize (mat3(modelViewMatrix) * vertex_normal);
	vec3 t = normalize (mat3(modelViewMatrix) * vertex_tangent);
	vec3 b = normalize (mat3(modelViewMatrix) * vertex_binormal);
	
	vec3 vertex_position_cameraspace = mat3(modelViewMatrix) * vertex_position;
	vec3 eye_direction_cameraspace = - vertex_position_cameraspace;
	
	vec3 light_position_cameraspace = mat3(viewMatrix) * light_position;
	vec3 light_direction_cameraspace = light_position_cameraspace + eye_direction_cameraspace;

	lightVector.x = dot(light_direction_cameraspace, t);
	lightVector.y = dot(light_direction_cameraspace, b);
	lightVector.z = dot(light_direction_cameraspace, n);
	lightVector = normalize(lightVector);
	
	vec3 half_direction_cameraspace = normalize(eye_direction_cameraspace + light_direction_cameraspace);
	halfVector.x = dot(half_direction_cameraspace, t);
	halfVector.y = dot(half_direction_cameraspace, b);
	halfVector.z = dot(half_direction_cameraspace, n);
	halfVector = normalize(halfVector);
	
	//testVector = eyeVector;
}
