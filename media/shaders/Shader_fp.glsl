uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;

uniform vec4 lightDiffuse;
uniform vec4 lightSpecular;

varying vec2 texCoords;

varying vec3 lightVector;
varying vec3 halfAngle;

varying vec3 testVector;

float saturate(float inValue)
{
   return clamp(inValue, 0.0, 1.0);
}

void main()
{
	//gl_FragColor = vec4(testVector, 1.0);
	gl_FragColor = texture2D(diffuseMap, texCoords);

	float specularPower = 2.0;

	vec4 diffuseColor = texture2D(diffuseMap, texCoords);
	vec4 normalColor = texture2D(normalMap, texCoords);
	vec4 specularColor = texture2D(specularMap, texCoords);
	
	vec3 lightVec = normalize(lightVector);
	vec3 halfVec = normalize(halfAngle);
	
	// get bump map vector, again expand from range-compressed
	vec3 bumpVec = normalize(normalColor.xyz * 2.0 - 1.0);

	float dot_l = dot(bumpVec, lightVec);
	float dot_h = dot(bumpVec, halfVec);
	
	float shininess = pow(max(dot_h, 0.0), specularPower);
	
	//Diffuse and bump
	//gl_FragColor = diffuseColor * lightDiffuse * saturate(dot_l);
	
	//Specular
	//gl_FragColor += specularColor * lightSpecular * shininess;
}