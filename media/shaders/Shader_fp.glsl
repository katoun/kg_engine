uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;

uniform vec4 lightDiffuse;
uniform vec4 lightSpecular;

varying vec2 texCoords;
varying vec3 lightDirection;
varying vec3 halfAngle;

void main()
{
	//Diffuse
	vec4 diffuseColor = texture2D(diffuseMap, texCoords);
	gl_FragColor = diffuseColor;

	//Bump
	vec4 normalColor = texture2D(normalMap, texCoords);
	// retrieve normalised light vector
	vec3 lightVec = normalize(lightDirection);
	// get bump map vector, again expand from range-compressed
	vec3 bumpVec = normalColor.xyz * 2.0 - 1.0;
	gl_FragColor *= lightDiffuse * clamp(dot(bumpVec, lightVec), 0.0, 1.0);
	
	//Specular
	vec4 specularColor = texture2D(specularMap, texCoords);
	// retrieve half angle and normalise
	vec3 halfVec = normalize(halfAngle);
	// Pre-raise the specular exponent to the eight power
	//float shininess = pow(clamp(dot(, halfVec), 0.0, 1.0), 4);
	float shininess = pow(max(dot(bumpVec, halfVec), 0.0), 2.0);
	gl_FragColor += specularColor * lightSpecular * shininess;
}