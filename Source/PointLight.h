#ifndef LIGHT_H
#define LIGHT_H
#include <glm\glm.hpp>
#include <GL/glew.h>
#include "GameObject.h"
class PointLight : public GameObject
{
public:
	//Parent inherited functions
	std::vector<glm::vec2> getPoints();
	glm::vec3 getPos() const;
	std::string type() const;
	//Own functions
	//1 / (linear * distance) + (quadratic * distance) 
	//aka decrease linear and quadratic values to make light reach further
	GLfloat linear;
	GLfloat quadratic;
	glm::vec3 pos;
	glm::vec3 colour;
	PointLight(glm::vec3 pos, glm::vec3 colour, float linear, float quadratic);
	PointLight(float x, float y, float z, glm::vec3 colour, float linear, float quadratic);
	PointLight(glm::vec3 pos, float r, float g, float b, float linear, float quadratic);
	PointLight(float x, float y, float z, float r, float g, float b, float linear, float quadratic);
	PointLight();
	~PointLight();
};
#endif