#version 440
const int MAX_JOINTS = 100;
const int MAX_WEIGHTS = 4;

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec2 vertexTexture;
layout (location = 2) in vec3 vertexNormal;

layout(location = 3) in vec4 vertexWeight;
layout(location = 4) in ivec4 vertexControllers;

uniform int hasAnimation;
uniform mat4 currentJointTrans[MAX_JOINTS];
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragPos;
out vec2 texCoords;
out vec3 normal;


void main()
{
	vec4 finalVertexPos = vec4(0.0f);
	vec4 finalNormal = vec4(0.0f);
	if(hasAnimation == 1)
	{
		//Cycle through all controllers for this vertex
		for (int i = 0; i < MAX_WEIGHTS; i++) 
		{
			mat4 jointTrans = currentJointTrans[vertexControllers[i]];
			vec4 posePos = jointTrans * vec4(vertexPos, 1.0f);
			finalVertexPos += posePos * vertexWeight[i];

			vec4 worldNormal = jointTrans * vec4(vertexNormal, 0.0f);
			finalNormal += worldNormal * vertexWeight[i];
		}
	}
	else
	{
		finalVertexPos = vec4(vertexPos,1.0f);
		finalNormal = vec4(normal,1.0f);
	}
	vec4 worldPos = model * finalVertexPos;
	//Position converted to clip space
	gl_Position = projection * view * worldPos;
	fragPos = worldPos.xyz;
	vec2 UV = vertexTexture;
	UV.y = (vertexTexture.y * -1.0f) + 1.0f;
	texCoords = UV;
	//Calculate normal
	mat3 normalMatrix = transpose(inverse(mat3(model)));
    normal = finalNormal.xyz;
}