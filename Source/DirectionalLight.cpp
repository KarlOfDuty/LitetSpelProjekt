#include"DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
	this->direction = glm::vec3(0,-1,0);
}

DirectionalLight::DirectionalLight(glm::vec3 dir)
{
	this->direction = dir;
}

DirectionalLight::~DirectionalLight()
{

}

void DirectionalLight::setDirection(glm::vec3 dir)
{
	this->direction = dir;
}

glm::vec3 DirectionalLight::getDirection()
{
	return direction;
}
