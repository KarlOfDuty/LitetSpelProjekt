#include "PlayerButterfly.h"

PlayerButterfly::PlayerButterfly(Model model, bool inWater) :PlayerChar(model, inWater)
{
	this->maxJumps = 1;
	this->jumpHeight = 10;
	box = new Model("models/cube/cubeGreen.obj");
}

PlayerButterfly::~PlayerButterfly()
{

}

void PlayerButterfly::operator=(const PlayerButterfly &originalObject)
{
	PlayerChar::operator=(originalObject);
}

int PlayerButterfly::getMaxJumps()
{
	return maxJumps;
}

float PlayerButterfly::getJumpHeight()
{
	return jumpHeight;
}

void PlayerButterfly::teleport()
{

}
void PlayerButterfly::shootAoe(std::vector<Model*> &allStaticModels, std::vector<Projectile*> &allProjectiles, glm::vec2 position)
{
	glm::vec2 direction(0, 1);
	glm::vec3 scale(3.0f, 2.0f, 1.0f);

	glm::vec3 ray_origin = glm::vec3(position, 0);
	glm::vec3 ray_direction(0, -1, 0);
	glm::vec3 aabb_min(-13, -13, -13);
	glm::vec3 aabb_max(13, 13, 13);
	float intersection_distance = 10000;
	for (int i = 0; i < allStaticModels.size(); i++)
	{
		glm::mat4 boxMatrix = allStaticModels[i]->getModelMatrix();
		if (collision::TestRayOBBIntersection(ray_origin, ray_direction, aabb_min, aabb_max, boxMatrix, intersection_distance))
		{
			if (intersection_distance == 0)
			{
				glm::vec3 newRay = ray_origin;
				newRay.y += 3;
				if (collision::TestRayOBBIntersection(newRay, ray_direction, aabb_min, aabb_max, boxMatrix, intersection_distance))
				{
					position.y += 3.0f - intersection_distance;
					i = allStaticModels.size();
				}
			}
			else
			{
				position.y -= intersection_distance;
				i = allStaticModels.size();
			}
		}
	}
	if (intersection_distance < 4)
	{
		Projectile* temp = new Projectile;
		temp->aoe(box, position, direction, glm::vec2(0.0f, 35.0f), 12.0f, scale);
		allProjectiles.push_back(temp);
	}
}
