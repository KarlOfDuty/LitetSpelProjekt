#ifndef COLLISION_H
#define COLLISION_H
#include <glm/gtx/transform.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
#include "Model.h"
namespace collision
{
    bool collision(std::vector<glm::vec2> points1, std::vector<glm::vec2> points2, glm::vec2 &mtv);
	bool collision(std::vector<glm::vec2> points1, std::vector<glm::vec2> points2);
	bool collision(std::vector<glm::vec2> points1, std::vector<glm::vec2> points2, glm::vec2 & mtv, glm::vec2 & collisionNormal);
	std::vector<glm::vec2> getAxis(std::vector<glm::vec2> allPoints);
    void projectOnAxis(std::vector<glm::vec2> allPoints, glm::vec2 thisAxis, float &min, float &max);
    float dot(glm::vec2 vec1, glm::vec2 vec2);
	glm::vec2 normalize(glm::vec2 theVector);
	bool isInside(std::vector<glm::vec2> object1Points, std::vector<glm::vec2> object2Points);
	bool TestRayOBBIntersection(glm::vec3 ray_origin, glm::vec3 ray_direction, glm::vec3 aabb_min, glm::vec3 aabb_max, glm::mat4 ModelMatrix, float& intersection_distance);
};
#endif