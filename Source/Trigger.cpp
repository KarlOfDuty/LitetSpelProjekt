#include "Trigger.h"
//Constructors
Trigger::Trigger()
{

}
Trigger::Trigger(glm::vec2 topLeft, glm::vec2 topRight, glm::vec2 bottomLeft, glm::vec2 bottomRight)
{
	this->corners[TOPLEFT] = topLeft;
	this->corners[TOPRIGHT] = topRight;
	this->corners[BOTTOMLEFT] = bottomLeft;
	this->corners[BOTTOMRIGHT] = bottomRight;
}
Trigger::Trigger(std::vector<glm::vec2> corners)
{
	this->corners = corners;
}
//Destructors
Trigger::~Trigger()
{

}