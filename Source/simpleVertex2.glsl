#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 lightSpaceMatrix2;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix2 * model * vec4(position, 1.0f);
}  