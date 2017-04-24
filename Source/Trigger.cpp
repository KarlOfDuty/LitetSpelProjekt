#include "Trigger.h"
std::vector<glm::vec2> Trigger::getCorners() const
{
	return corners;
}
//Constructors
Trigger::Trigger()
{

}
Trigger::Trigger(glm::vec2 topLeft, glm::vec2 topRight, glm::vec2 bottomLeft, glm::vec2 bottomRight)
{
	corners = std::vector<glm::vec2>(4);
	this->corners[0] = topLeft;
	this->corners[1] = topRight;
	this->corners[2] = bottomLeft;
	this->corners[3] = bottomRight;
}
Trigger::Trigger(std::vector<glm::vec2> corners)
{
	this->corners = corners;
}
//Destructors
Trigger::~Trigger()
{

}