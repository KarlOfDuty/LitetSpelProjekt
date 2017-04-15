#ifndef LIGHT_H
#define LIGHT_H
#include <glm\glm.hpp>
class Light
{
private:
public:
	glm::vec3 pos;
	glm::vec3 colour;
	Light(glm::vec3 pos, glm::vec3 colour);
	Light(float x, float y, float z, glm::vec3 colour);
	Light(glm::vec3 pos, float r, float g, float b);
	Light(float x, float y, float z, float r, float g, float b);
	Light();
	~Light();
};
#endif