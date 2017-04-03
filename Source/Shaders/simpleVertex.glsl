#version 440
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
 
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vertexColor;

void main()
{
	vec4 worldPos = model * vec4(position, 1.0f);
    gl_Position = projection * view * worldPos;
    vertexColor = vec4(color, 1.0f);
}