#ifndef LIGHT_H
#define LIGHT_H
#include <glm\glm.hpp>
#include <GL/glew.h>
class Light
{
public:
	//1 / (linear * distance) + (quadratic * distance) 
	//aka decrease values to make light reach further
	GLfloat linear;
	GLfloat quadratic;
	glm::vec3 pos;
	glm::vec3 colour;
	Light(glm::vec3 pos, glm::vec3 colour, float linear, float quadratic);
	Light(float x, float y, float z, glm::vec3 colour, float linear, float quadratic);
	Light(glm::vec3 pos, float r, float g, float b, float linear, float quadratic);
	Light(float x, float y, float z, float r, float g, float b, float linear, float quadratic);
	Light();
	~Light();
};
#endif