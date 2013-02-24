#version 330

in vec2 uv;

in vec3 light_vector; // Tangent-space, normalized
in vec3 half_vector; // Tangent-space, normalized

in vec3 testVector;

out vec4 fragColor;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;

uniform float specular_power;
uniform vec4 light_diffuse;
uniform vec4 light_specular;

float saturate(float inValue)
{
   return clamp(inValue, 0.0, 1.0);
}

void main()
{
	//fragColor = vec4(testVector, 1.0);
	//fragColor = texture2D(diffuseMap, uv);

	vec4 diffuseColor = texture2D(diffuseMap, uv);
	vec4 ambientColor = vec4(0.1,0.1,0.1,1.0) * diffuseColor;
	vec4 normalColor = texture2D(normalMap, uv);
	vec4 specularColor = texture2D(specularMap, uv);

	vec3 normal_tangentspace = normalize(normalColor.xyz * 2.0 - 1.0);

	float dot_l = dot(normal_tangentspace, light_vector);
	float dot_h = dot(normal_tangentspace, half_vector);
	
	float lamberFactor = saturate(dot_l);
	float shininess = pow(max(dot_h, 0.0), specular_power);

	fragColor = ambientColor;
	fragColor += diffuseColor * light_diffuse * lamberFactor;
	fragColor += specularColor * light_specular * shininess;
}