#version 440
out vec4 fragColor;
in vec2 texCoords;

const int MAX_NR_POINT_LIGHTS = 5;
const int MAX_NR_DIR_LIGHTS = 5;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gAmbient;
uniform sampler2D depthMap[MAX_NR_DIR_LIGHTS];

struct PointLight 
{
    vec3 position;
    vec3 colour;
    float linear;
    float quadratic;
};
struct DirLight 
{
    vec3 dir;
    vec3 colour;
};
uniform PointLight pointLights[MAX_NR_POINT_LIGHTS];
uniform DirLight directionalLights[MAX_NR_DIR_LIGHTS];
uniform int numberOfPointLights;
uniform int numberOfDirLights;
uniform vec3 viewPos;
uniform mat4 lightSpaceMatrix[MAX_NR_DIR_LIGHTS];

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDirection, sampler2D depthM)
{
	//Perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	//Transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;
	//Get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;
	float shadow = 0.0;
	// Calculate bias
	float bias = max(0.05 * (1.0 - dot(normal, lightDirection)), 0.005);   
	//PCF - Percentage-Closer Filtering - used to offset the texture coordinates
	vec2 texelSize = 1.0 / textureSize(depthM, 0);

	//Check whether current frag pos is in shadow
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
	//Sample size - increase to improve shadow quality
	shadow /= 64.0;

	return shadow;
}

void main()
{             
    //Retrieve data from G-buffer	
    vec3 fragPos = texture(gPosition, texCoords).rgb;
	vec3 normal = texture(gNormal, texCoords).rgb;
    vec3 diffuse = texture(gAlbedoSpec, texCoords).rgb;
	float specular = texture(gAlbedoSpec, texCoords).a;
	vec3 ambient = texture(gAmbient, texCoords).rgb;
	//Ambient is not used because of reasons
	vec3 lighting = diffuse*0.3f;
	vec3 viewDir = normalize(viewPos - fragPos);

	for(int i = 0; i < numberOfDirLights; i++)
	{	
		vec4 lightSpace;
		lightSpace = lightSpaceMatrix[i] * vec4(fragPos, 1.0);
		// Calculate shadows
		float shadow = ShadowCalculation(lightSpace, normal, directionalLights[i].dir, depthMap[i]);
		lighting += (1.0 - shadow) * (diffuse*directionalLights[i].colour);
	}
	for(int i = 0; i < numberOfPointLights; i++)
	{
		//Attenuation
		float lightDistance = length(pointLights[i].position - fragPos);
        float attenuation = 1.0 / (1.0 + pointLights[i].linear * lightDistance + pointLights[i].quadratic * lightDistance * lightDistance);
		vec3 lightDir = normalize(pointLights[i].position - fragPos);
		vec3 thisDiffuse = max(dot(normal, lightDir), 0.0) * diffuse * pointLights[i].colour;
		//Specular
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
		vec3 thisSpecular = pointLights[i].colour * spec * specular;
        thisDiffuse *= attenuation;
        thisSpecular *= attenuation;
		lighting += (thisDiffuse + thisSpecular);
	}
	fragColor = vec4(lighting, 1.0f);
	float depthValue = texture(depthMap[0],texCoords).r;
	//Test depthmap
	//fragColor = vec4(vec3(depthValue),1.0);

}

