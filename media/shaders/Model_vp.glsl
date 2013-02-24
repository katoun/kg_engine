#version 330

in vec3 vertex_position;
in vec3 vertex_normal;
in vec3 vertex_tangent;
in vec3 vertex_binormal;
in vec2 vertex_uv;

out vec2 uv;

out vec3 light_vector; // Tangent-space, normalized
out vec3 half_vector; // Tangent-space, normalized

out vec3 testVector;

uniform mat4 modelViewProjMatrix;
uniform mat4 modelMatrix;

uniform vec3 light_position; // World-space
uniform vec3 camera_position; // World-space

void main()
{
	gl_Position = modelViewProjMatrix * vec4(vertex_position, 1.0);
	
	uv = vertex_uv;
	
	// Building the matrix World Space -> Tangent Space
	vec3 n = normalize (mat3(modelMatrix) * vertex_normal);
	vec3 t = normalize (mat3(modelMatrix) * vertex_tangent);
	vec3 b = normalize (mat3(modelMatrix) * vertex_binormal);
	
	vec3 vertex_position_worldspace = mat3(modelMatrix) * vertex_position;
	vec3 eye_direction_worldspace = - vertex_position_worldspace;

	vec3 light_direction_worldspace = light_position - vertex_position_worldspace;

	light_vector.x = dot(light_direction_worldspace, t);
	light_vector.y = dot(light_direction_worldspace, b);
	light_vector.z = dot(light_direction_worldspace, n);
	light_vector = normalize(light_vector);
	
	vec3 half_direction_worldspace = eye_direction_worldspace + light_direction_worldspace;
	half_vector.x = dot(half_direction_worldspace, t);
	half_vector.y = dot(half_direction_worldspace, b);
	half_vector.z = dot(half_direction_worldspace, n);
	half_vector = normalize(half_vector);
	
	//testVector = eyeVector;
}
