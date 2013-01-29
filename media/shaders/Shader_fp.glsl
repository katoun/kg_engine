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
	vec4 diffuseColor = texture2D(diffuseMap, texCoords);
	vec4 normalColor = texture2D(normalMap, texCoords);
	vec4 specularColor = texture2D(specularMap, texCoords);
	
	gl_FragColor = diffuseColor + normalColor + specularColor;
}