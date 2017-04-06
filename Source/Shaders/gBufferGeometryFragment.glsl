#version 440
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout(location = 3) out vec3 gAmbient;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalMap;
uniform sampler2D ambientTexture;

in vec2 texCoords;
in vec3 fragPos;
in vec3 normal;

void main()
{
	gPosition = fragPos;
	gNormal = normalize(normal);
	gAlbedoSpec.rgb = texture(diffuseTexture, texCoords).rgb;
	gAlbedoSpec.a = texture(specularTexture, texCoords).r;
	gAmbient = texture(ambientTexture,texCoords).rgb;
} 