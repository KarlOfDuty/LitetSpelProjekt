#include"DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
	this->direction = glm::vec3(0,-1,0);
	this->colour = glm::vec3(0.5f,0.5f,0.5f);
}

DirectionalLight::DirectionalLight(glm::vec3 dir, glm::vec3 colour)
{
	this->direction = dir;
	this->colour = colour;
}

DirectionalLight::~DirectionalLight()
{

}

void DirectionalLight::setDirection(glm::vec3 dir)
{
	this->direction = dir;
}

void DirectionalLight::setColour(glm::vec3 colour)
{
	this->colour = colour;
}

glm::vec3 DirectionalLight::getDirection()
{
	return direction;
}

glm::vec3 DirectionalLight::getColour()
{
	return colour;
}
