#version 440

const int maxJoints = 100;
const int maxWeights = 4;

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_UV;
layout(location = 2) in vec3 vertex_normal;

layout(location = 3) in vec4 vertex_weight;
layout(location = 4) in ivec4 vertex_controllers;


out vec2 outUVs;
out vec3 outNorm;

layout(binding = 3 , std140) uniform uniformBlock
{
	vec3 camPos;
	mat4 world;
	mat4 LookAt;
	mat4 projection;
	mat4 MVP;
};

uniform mat4 currentJointTrans[100];

void main() {
	
	vec4 finalModelPos = vec4(0.0);
	vec4 finalNormal = vec4(0.0);

	for (int i = 0; i < 4; i++)
	{
		mat4 jointTrans = currentJointTrans[vertex_controllers[i]];
		vec4 posePos = jointTrans * vec4(vertex_position, 1.0);
		finalModelPos += posePos * vertex_weight[i];
	
		vec4 worldNormal = jointTrans * vec4(vertex_normal, 0.0);
		finalNormal += worldNormal * vertex_weight[i];
	}

	gl_Position = MVP * finalModelPos;
	outNorm = finalNormal.xyz;

	outUVs = vertex_UV;

}