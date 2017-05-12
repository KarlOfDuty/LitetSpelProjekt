#include "PlayerButterfly.h"

PlayerButterfly::PlayerButterfly(Model model, bool inWater) :PlayerChar(model, inWater)
{
	this->maxJumps = 1;
	this->jumpHeight = 15;
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
	glm::vec3 scale(4.0f, 0.1f, 1.0f);

	glm::vec3 ray_origin = glm::vec3(position, 0);
	glm::vec3 ray_direction(0, -1, 0);
	glm::vec3 aabb_min(-13, -13, -13);
	glm::vec3 aabb_max(13, 13, 13);
	float intersection_distance = 10000;
	//Check how many arrows are active in the arrow vector
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
	int activeArrows = 0;
	for (int i = 0; i < allProjectiles.size(); i++)
	{
		if (allProjectiles[i]->isInUse())
			activeArrows++;
	}
	if (intersection_distance < 4)
	{
		if (activeArrows < allProjectiles.size())
		{
			if (activeArrows < allProjectiles.size())
			{
				for (int i = 0; i < allProjectiles.size(); i++)
				{
					if (!allProjectiles[i]->isInUse())
					{
						allProjectiles[i]->aoe(box, position, direction, 5.0f, scale);
						i = (int)allProjectiles.size();
					}
				}
			}
		}
		else
		{
			Projectile* temp = new Projectile;
			temp->aoe(box, position, direction, 5.0f, scale);
			allProjectiles.push_back(temp);
		}
	}
}
