#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H
#include <glm\glm.hpp>
class DirectionalLight
{
public:
	DirectionalLight();
	DirectionalLight(glm::vec3 dir, glm::vec3 colour);
	~DirectionalLight();
	void setDirection(glm::vec3 dir);
	void setColour(glm::vec3 colour);
	glm::vec3 getDirection();
	glm::vec3 getColour();
private:
	glm::vec3 direction;
	glm::vec3 colour;
};
#endif