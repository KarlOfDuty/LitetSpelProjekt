#include "PointLight.h"
std::vector<glm::vec2> PointLight::getPoints()
{
	std::vector<glm::vec2> point = { glm::vec2(pos.x,pos.y) };
	return point;
}
glm::vec3 PointLight::getPos() const
{
	return pos;
}
std::string PointLight::type() const
{
	return "PointLight";
}
PointLight::PointLight(glm::vec3 pos, glm::vec3 colour, float linear, float quadratic)
{
	this->pos = pos;
	this->colour = colour;
	this->linear = linear;
	this->quadratic = quadratic;
}
PointLight::PointLight(float x, float y, float z, glm::vec3 colour, float linear, float quadratic)
{
	this->pos = glm::vec3(x, y, z);
	this->colour = colour;
	this->linear = linear;
	this->quadratic = quadratic;
}
PointLight::PointLight(glm::vec3 pos, float r, float g, float b, float linear, float quadratic)
{
	this->pos = pos;
	this->colour = glm::vec3(r, g, b);
	this->linear = linear;
	this->quadratic = quadratic;
}
PointLight::PointLight(float x, float y, float z, float r, float g, float b, float linear, float quadratic)
{
	this->pos = glm::vec3(x, y, z);
	this->colour = glm::vec3(r, g, b);
	this->linear = linear;
	this->quadratic = quadratic;
}
PointLight::PointLight()
{
	this->pos = glm::vec3(0,0,0);
	this->colour = glm::vec3(0,0,0);
	this->linear = 0.0f;
	this->quadratic = 0.0f;
}

PointLight::~PointLight()
{

}