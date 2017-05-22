#version 440

const int maxJoints = 100;

in vec2 outUVs;
in vec3 outNorm;
uniform sampler2D DiffuseMap;
uniform sampler2D NormalMap;

layout (location = 0) out vec4 fragment_color;

layout(binding = 3 , std140) uniform uniformBlock
{
	vec3 camPos;
	mat4 world;
	mat4 LookAt;
	mat4 projection;
	mat4 MVP;
};

void main()
{
	//Lights
	vec3 lightPos = vec3(0.0f,100.0f, 30.0f);	//Puts out the light pos in the world
	vec3 lightVec = lightPos - vec3(10.0, 10.0, 10.0);	//Calculates the direction the light is going. In our case, to the origin of the world
	lightVec = normalize(lightVec);				//We normalize this
	vec3 lightColor = vec3(1.0f,1.0f,1.0f);		//We enter the color of the light. In our case, white

	//vec3 normal = texture(NormalMap, outUVs).rgb;
	//normal = normalize(normal * 2 - 1.0);
	//
	////Elias Diffuse Color
	//vec3 diffuseColor = texture(DiffuseMap, outUVs).rgb;
	//
	////Elias Ambient
	//vec3 fAmbient = 0.2 * diffuseColor;
	//
	////Elias Diffuse
	//float diff = max(dot(lightVec, normal),0.0f);
	//vec3 fDiffuse = diff * diffuseColor;

	//fragment_color = vec4(fAmbient + fDiffuse + 0.0, 1.0f);
	fragment_color = vec4(0.5f, 0.5f, 0.5f, 1.0f);

}