#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H
#include <glm\glm.hpp>
class DirectionalLight
{
public:
	DirectionalLight();
	DirectionalLight(glm::vec3 dir);
	~DirectionalLight();
	void setDirection(glm::vec3 dir);
	glm::vec3 getDirection();
private:
	glm::vec3 direction;
};
#endif