#include "Light.h"
std::vector<glm::vec2> Light::getPoints()
{
	std::vector<glm::vec2> point = { glm::vec2(pos.x,pos.y) };
	return point;
}
glm::vec3 Light::getPos() const
{
	return pos;
}
Light::Light(glm::vec3 pos, glm::vec3 colour, float linear, float quadratic)
{
	this->pos = pos;
	this->colour = colour;
	this->linear = linear;
	this->quadratic = quadratic;
}
Light::Light(float x, float y, float z, glm::vec3 colour, float linear, float quadratic)
{
	this->pos = glm::vec3(x, y, z);
	this->colour = colour;
	this->linear = linear;
	this->quadratic = quadratic;
}
Light::Light(glm::vec3 pos, float r, float g, float b, float linear, float quadratic)
{
	this->pos = pos;
	this->colour = glm::vec3(r, g, b);
	this->linear = linear;
	this->quadratic = quadratic;
}
Light::Light(float x, float y, float z, float r, float g, float b, float linear, float quadratic)
{
	this->pos = glm::vec3(x, y, z);
	this->colour = glm::vec3(r, g, b);
	this->linear = linear;
	this->quadratic = quadratic;
}
Light::Light()
{
	this->pos = glm::vec3(0,0,0);
	this->colour = glm::vec3(0,0,0);
	this->linear = 0.0f;
	this->quadratic = 0.0f;
}

Light::~Light()
{

}