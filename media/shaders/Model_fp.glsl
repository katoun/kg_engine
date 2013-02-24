#version 330

in vec2 uv;

in vec3 lightVector;
in vec3 halfVector;

in vec3 testVector;

out vec4 fragColor;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;

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

	float specularPower = 5.0;

	vec4 diffuseColor = texture2D(diffuseMap, uv);
	vec4 ambientColor = vec4(0.1,0.1,0.1,1.0) * diffuseColor;
	vec4 normalColor = texture2D(normalMap, uv);
	vec4 specularColor = texture2D(specularMap, uv);
	
	vec3 lightVec = normalize(lightVector);
	vec3 halfVec = normalize(halfVector);

	vec3 normal_tangentspace = normalize(normalColor.xyz * 2.0 - 1.0);

	float dot_l = dot(normal_tangentspace, lightVec);
	float dot_h = dot(normal_tangentspace, halfVec);
	
	float lamberFactor = saturate(dot_l);
	float shininess = pow(max(dot_h, 0.0), specularPower);

	fragColor = ambientColor;
	//fragColor += diffuseColor * light_diffuse * lamberFactor;
	fragColor += diffuseColor * vec4(1,1,1,1) * lamberFactor;
	fragColor += specularColor * light_specular * shininess;
}