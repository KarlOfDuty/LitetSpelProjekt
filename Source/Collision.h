#ifndef COLLISION_H
#define COLLISION_H
#include <glm/gtx/transform.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
namespace collision
{
    bool fixCollision(std::vector<glm::vec2> points1, std::vector<glm::vec2> points2, glm::vec2 &mtv);
    std::vector<glm::vec2> getAxis(std::vector<glm::vec2> allPoints);
    void projectOnAxis(std::vector<glm::vec2> allPoints, glm::vec2 thisAxis, float &min, float &max);
    float dot(glm::vec2 vec1, glm::vec2 vec2);
	glm::vec2 normalize(glm::vec2 theVector);
};
#endif