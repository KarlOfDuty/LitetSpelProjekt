#version 440
out vec4 fragColor;
in vec2 texCoords;

const int MAX_NR_LIGHTS = 2;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gAmbient;
uniform sampler2D depthMap[MAX_NR_LIGHTS];

struct light 
{
    vec3 position;
    vec3 color;
    float linear;
    float quadratic;
};
uniform light lights[MAX_NR_LIGHTS];
uniform vec3 viewPos;
uniform mat4 lightSpaceMatrix[MAX_NR_LIGHTS];

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDirection, sampler2D depthM)
{
	// Perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// Transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;
	// Get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;
	float shadow = 0.0;
	// Calculate bias
	float bias = max(0.05 * (1.0 - dot(normal, lightDirection)), 0.005);   
	// PCF - Percentage-Closer Filtering - used to offset the texture coordinates
	vec2 texelSize = 1.0 / textureSize(depthM, 0);

	// Check wheter current frag pos is in shadow
	for(int x = -1; x <= 1; x++)
	{
		for(int y = -1; y <= 1; y++)
		{
			float pcfDepth = texture(depthM, projCoords.xy + vec2(x, y) * texelSize).r;
			if(currentDepth - bias > pcfDepth)
			{
				shadow += 1.0;
			}
		}
	}
	// Sample size - increase to improve shadow quality
	shadow /= 18.0;

	return shadow;
}

void main()
{             
    // Retrieve data from G-buffer	
    vec3 fragPos = texture(gPosition, texCoords).rgb;
	vec3 normal = texture(gNormal, texCoords).rgb;
    vec3 diffuse = texture(gAlbedoSpec, texCoords).rgb;
	float specular = texture(gAlbedoSpec, texCoords).a;
	vec3 ambient = texture(gAmbient, texCoords).rgb;
	//Adds the ambient
	vec3 lighting = diffuse*0.3f;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec4 lightSpaces[3];
	lightSpaces[0] = lightSpaceMatrix[0] * vec4(fragPos, 1.0);
	lightSpaces[1] = lightSpaceMatrix[1] * vec4(fragPos, 1.0);
	vec4 fragPosLightSpace[2];
	fragPosLightSpace[0] = lightSpaceMatrix[0] * vec4(fragPos, 1.0);
	fragPosLightSpace[1] = lightSpaceMatrix[1] * vec4(fragPos, 1.0);
	 
	//TODO: Get actual number of lights for this loop
	for(int i = 0; i < MAX_NR_LIGHTS; ++i)
	{
		//Attenuation
		float lightDistance = length(lights[i].position - fragPos);
        float attenuation = 1.0 / (1.0 + lights[i].linear * lightDistance + lights[i].quadratic * lightDistance * lightDistance);
		vec3 lightDir = normalize(lights[i].position - fragPos);
		vec3 thisDiffuse = max(dot(normal, lightDir), 0.0) * diffuse * lights[i].color;
		//Specular
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
		vec3 thisSpecular = lights[i].color * spec * specular;
		// Calculate shadows
		float shadow = ShadowCalculation(lightSpaces[i], normal, lightDir, depthMap[i]);
        thisDiffuse *= attenuation;
        thisSpecular *= attenuation;
		lighting += (1.0 - shadow) * (thisDiffuse + thisSpecular);
	}
	fragColor = vec4(lighting, 1.0f);
	float depthValue = texture(depthMap[0],texCoords).r;
	float depthValue2 = texture(depthMap[1],texCoords).r;
	// Test depthmap
	//fragColor = vec4(vec3(depthValue),1.0);

}

