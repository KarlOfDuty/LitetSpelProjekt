#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <glm\glm.hpp>
#include <vector>
class GameObject
{
public:
	virtual glm::vec3 getPos() const = 0;
	virtual std::vector<glm::vec2> getPoints() = 0;
	GameObject();
	~GameObject();
};
#endif