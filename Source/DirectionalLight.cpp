#include"DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
	this->direction = glm::vec3(0.0f,-1.0f,0.0f);
	this->colour = glm::vec3(0.5f,0.5f,0.5f);
}

DirectionalLight::DirectionalLight(glm::vec3 dir, glm::vec3 colour)
{
	this->direction = glm::normalize(dir);
	this->colour = colour;
}

DirectionalLight::~DirectionalLight()
{

}

void DirectionalLight::setDirection(glm::vec3 dir)
{
	this->direction = glm::normalize(dir);
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
