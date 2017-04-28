#include "Collision.h"

bool collision::testCollision(std::vector<glm::vec2> points1, std::vector<glm::vec2> points2, glm::vec2 &mtv)
{
	float o = 1000000000;
	glm::vec2 smallestAxis;
	std::vector<glm::vec2> axis1 = getAxis(points1);
	std::vector<glm::vec2> axis2 = getAxis(points2);

	for (int i = 0; i < axis1.size(); i++) {
		glm::vec2 thisAxis = axis1[i];

		float s1min, s1max;
		projectOnAxis(points1, thisAxis, s1min, s1max);
		float s2min, s2max;
		projectOnAxis(points2, thisAxis, s2min, s2max);

		if (s2min > s1max || s2max < s1min) {
			return false;
		}

		float overlap;
		if (s1max < s2min) {
			overlap = s1max - s2min;
		}
		else
		{
			overlap = s2max - s1min;
		}
		if (overlap < o)
		{
			o = overlap;
			smallestAxis = thisAxis;
		}
	}
	for (int i = 0; i < axis2.size(); i++) {
		glm::vec2 thisAxis = axis2[i];

		float s1min, s1max;
		projectOnAxis(points1, thisAxis, s1min, s1max);
		float s2min, s2max;
		projectOnAxis(points2, thisAxis, s2min, s2max);

		if (s2max < s1min || s1max < s2min) {
			return false;
		}

		float overlap;
		if (s1max < s2min) {
			overlap = s1max - s2min;
		}
		else
		{
			overlap = s2max - s1min;
		}
		if (overlap < o)
		{
			o = overlap;
			smallestAxis = thisAxis;
		}
	}
	mtv = smallestAxis*o;
	if (mtv == glm::vec2())
	{
		return false;
	}
	return true;
}

bool collision::testCollision(std::vector<glm::vec2> points1, std::vector<glm::vec2> points2)
{
	glm::vec2 mtv = glm::vec2();
	float o = 1000000000;
	glm::vec2 smallestAxis;
	std::vector<glm::vec2> axis1 = getAxis(points1);
	std::vector<glm::vec2> axis2 = getAxis(points2);

	for (int i = 0; i < axis1.size(); i++) 
	{
		glm::vec2 thisAxis = axis1[i];

		float s1min, s1max;
		projectOnAxis(points1, thisAxis, s1min, s1max);
		float s2min, s2max;
		projectOnAxis(points2, thisAxis, s2min, s2max);

		if (s2min > s1max || s2max < s1min) 
		{
			return false;
		}

		float overlap;
		if (s1max < s2min) 
		{
			overlap = s1max - s2min;
		}
		else
		{
			overlap = s2max - s1min;
		}
		if (overlap < o)
		{
			o = overlap;
			smallestAxis = thisAxis;
		}
	}
	for (int i = 0; i < axis2.size(); i++) 
	{
		glm::vec2 thisAxis = axis2[i];

		float s1min, s1max;
		projectOnAxis(points1, thisAxis, s1min, s1max);
		float s2min, s2max;
		projectOnAxis(points2, thisAxis, s2min, s2max);

		if (s2max < s1min || s1max < s2min) 
		{
			return false;
		}

		float overlap;
		if (s1max < s2min) 
		{
			overlap = s1max - s2min;
		}
		else
		{
			overlap = s2max - s1min;
		}
		if (overlap < o)
		{
			o = overlap;
			smallestAxis = thisAxis;
		}
	}
	mtv = smallestAxis*o;
	if (mtv == glm::vec2())
	{
		return false;
	}
	return true;
}

std::vector<glm::vec2> collision::getAxis(std::vector<glm::vec2> allPoints)
{
	std::vector<glm::vec2> axis;
	for (int i = 0; i < allPoints.size(); i++) {
		glm::vec2 p1 = allPoints[i];
		glm::vec2 p2;
		if (i + 1 < allPoints.size())
			p2 = allPoints[i + 1];
		else
			p2 = allPoints[0];

		glm::vec2 edge = p1 - p2;

		glm::vec2 normal(-edge.y, edge.x);

		axis.push_back(normalize(normal));
	}
	return axis;
}
void collision::projectOnAxis(std::vector<glm::vec2> allPoints, glm::vec2 thisAxis, float &min, float &max)
{
	min = dot(thisAxis, allPoints[0]);
	max = min;
	for (int i = 1; i < allPoints.size(); i++) {
		float p = dot(thisAxis, allPoints[i]);
		if (p < min) min = p;
		else if (p > max) max = p;
	}
}
float collision::dot(glm::vec2 vec1, glm::vec2 vec2)
{
	return vec1.x*vec2.x + vec1.y*vec2.y;
}
glm::vec2 collision::normalize(glm::vec2 thisVector)
{
	glm::vec2 normalized;
	float length = sqrt(pow(thisVector.x, 2) + pow(thisVector.y, 2));
	normalized.x = thisVector.x / length;
	normalized.y = thisVector.y / length;
	return normalized;
}
bool collision::isInside(std::vector<glm::vec2> object1Points, std::vector<glm::vec2> object2Points)
{
	glm::vec2 object1Min = glm::vec2(1000000);
	glm::vec2 object1Max = glm::vec2(-1000000);
	for (int i = 0; i < object1Points.size(); i++)
	{
		if (object1Points[i].x < object1Min.x) object1Min.x = object1Points[i].x;
		if (object1Points[i].y < object1Min.y) object1Min.y = object1Points[i].y;
		if (object1Points[i].x > object1Max.x) object1Max.x = object1Points[i].x;
		if (object1Points[i].y > object1Max.y) object1Max.y = object1Points[i].y;
	}

	glm::vec2 object2Min = glm::vec2(1000000);
	glm::vec2 object2Max = glm::vec2(-1000000);
	for (int i = 0; i < object2Points.size(); i++)
	{
		if (object2Points[i].x < object2Min.x) object2Min.x = object2Points[i].x;
		if (object2Points[i].y < object2Min.y) object2Min.y = object2Points[i].y;
		if (object2Points[i].x > object2Max.x) object2Max.x = object2Points[i].x;
		if (object2Points[i].y > object2Max.y) object2Max.y = object2Points[i].y;
	}

	if (object1Max.x > object2Min.x &&
		object1Min.x < object2Max.x &&
		object1Max.y > object2Min.y &&
		object1Min.y < object2Max.y)
	{
		return true;
	}

	return false;
}