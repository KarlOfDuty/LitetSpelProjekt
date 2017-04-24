#ifndef TRIGGER_H
#define TRIGGER_H
#include <glm\glm.hpp>
#include <vector>
enum {TOPLEFT, TOPRIGHT, BOTTOMLEFT, BOTTOMRIGHT};
class Trigger
{
private:
	std::vector<glm::vec2> corners;
public:
	std::vector<glm::vec2> getCorners() const;
	Trigger();
	Trigger(glm::vec2 topLeft, glm::vec2 topRight, glm::vec2 bottomLeft, glm::vec2 bottomRight);
	Trigger(std::vector<glm::vec2> corners);
	~Trigger();
};
#endif