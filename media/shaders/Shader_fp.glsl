#version 330

in vec2 uv;

in vec3 lightVector;
in vec3 halfAngle;

in vec3 testVector;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;

uniform vec4 lightDiffuse;
uniform vec4 lightSpecular;

out vec4 fragColor;

float saturate(float inValue)
{
   return clamp(inValue, 0.0, 1.0);
}

void main()
{
	//fragColor = vec4(testVector, 1.0);
	//fragColor = texture2D(diffuseMap, uv);

	float specularPower = 2.0;

	vec4 diffuseColor = texture2D(diffuseMap, uv);
	vec4 normalColor = texture2D(normalMap, uv);
	vec4 specularColor = texture2D(specularMap, uv);
	
	vec3 lightVec = normalize(lightVector);
	vec3 halfVec = normalize(halfAngle);
	
	// get bump map vector, again expand from range-compressed
	vec3 bumpVec = normalize(normalColor.xyz * 2.0 - 1.0);

	float dot_l = dot(bumpVec, lightVec);
	float dot_h = dot(bumpVec, halfVec);
	
	float shininess = pow(max(dot_h, 0.0), specularPower);
	
	//Diffuse and bump
	fragColor = diffuseColor * lightDiffuse * saturate(dot_l);
	
	//Specular
	fragColor += specularColor * lightSpecular * shininess;
}