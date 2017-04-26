#include "Trigger.h"
template<class T>
std::vector<glm::vec2> Trigger<T>::getCorners() const
{
	return corners;
}
template<class T>
bool Trigger<T>::update()
{
	//Find how many models intersect this one
	for (int i = 0; i < activators.size(); i++)
	{
		collision::testCollision(corners,activators[i]->getPoints());
	}
}
template<class T>
void Trigger<T>::activate()
{

}
//Constructors
template<class T>
Trigger<T>::Trigger()
{

}
template<class T>
Trigger<T>::Trigger(glm::vec2 topLeft, glm::vec2 topRight, glm::vec2 bottomLeft, glm::vec2 bottomRight)
{
	corners = std::vector<glm::vec2>(4);
	this->corners[0] = topLeft;
	this->corners[1] = topRight;
	this->corners[2] = bottomLeft;
	this->corners[3] = bottomRight;
}
template<class T>
Trigger<T>::Trigger(std::vector<glm::vec2> corners)
{
	this->corners = corners;
}
//Destructors
template<class T>
Trigger<T>::~Trigger()
{

}