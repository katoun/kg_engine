uniform sampler2D diffuseMap;

varying vec2 texCoords;

void main()
{
	vec4 diffuse = texture2D(diffuseMap, texCoords);
	
	gl_FragColor = diffuse;
}