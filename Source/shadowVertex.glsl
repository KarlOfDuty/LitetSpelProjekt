#version 330 core
layout (location = 0) in vec3 position;

const int MAX_NR_LIGHTS = 5;
uniform mat4 lightSpaceMatrix[MAX_NR_LIGHTS];
uniform int index;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix[index] * model * vec4(position, 1.0f);
}  