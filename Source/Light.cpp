#include "Light.h"
Light::Light(glm::vec3 pos, glm::vec3 colour)
{
	this->pos = pos;
	this->colour = colour;
}
Light::Light(float x, float y, float z, glm::vec3 colour)
{
	this->pos = glm::vec3(x, y, z);
	this->colour = colour;
}
Light::Light(glm::vec3 pos, float r, float g, float b)
{
	this->pos = pos;
	this->colour = glm::vec3(r, g, b);
}
Light::Light(float x, float y, float z, float r, float g, float b)
{
	this->pos = glm::vec3(x, y, z);
	this->colour = glm::vec3(r, g, b);
}
Light::Light()
{
	this->pos = glm::vec3(0,0,0);
	this->colour = glm::vec3(0,0,0);
}

Light::~Light()
{

}