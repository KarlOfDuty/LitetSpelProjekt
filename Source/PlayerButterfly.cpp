#include "PlayerButterfly.h"

PlayerButterfly::PlayerButterfly(Model* model, bool inWater) :PlayerChar(model, inWater)
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
	//not used
}
void PlayerButterfly::shootAoe(std::vector<Model*> &allStaticModels, std::vector<Projectile*> &allProjectiles, glm::vec2 position)
{
	//Check how many arrows are active in the arrow vector
	if (attackCooldown.getElapsedTime().asSeconds() > 0.5f)
	{
		//Sort models by x axis
		std::vector<Model*> sortedModels;
		for (int i = 0; i < allStaticModels.size(); i++)
		{
			glm::vec3 min, max;
			allStaticModels[i]->getScaledMinMaxBouding(min, max);
			min += allStaticModels[i]->getPos();
			max += allStaticModels[i]->getPos();
			if (position.x >= min.x && position.x <= max.x)
			{
				sortedModels.push_back(allStaticModels[i]);
			}
		}
		glm::vec2 direction(0, 1);
		glm::vec3 scale(4.0f, 0.1f, 1.0f);

		//Raycast to ground to see find y pos
		glm::vec3 ray_origin = glm::vec3(position.x, position.y + 0.001, 0);
		glm::vec3 ray_direction(0, -1, 0);
		glm::vec3 boxScale;
		glm::vec3 aabb_min;
		glm::vec3 aabb_max;
		float intersection_distance;
		float minDist = 10000;
		for (int i = 0; i < sortedModels.size(); i++)
		{
			glm::mat4 boxMatrix = sortedModels[i]->getModelMatrix();
			sortedModels[i]->getMinMaxBouding(aabb_min, aabb_max);
			glm::decompose(boxMatrix, boxScale, glm::quat(), glm::vec3(), glm::vec3(), glm::vec4());
			aabb_max *= boxScale*boxScale;
			aabb_min *= boxScale*boxScale;
			if (collision::TestRayOBBIntersection(ray_origin, ray_direction, aabb_min, aabb_max, boxMatrix, intersection_distance))
			{
				if (intersection_distance < minDist)
				{
					minDist = intersection_distance;
				}
			}
		}
		//If inside try to go up 4 y coords and try again
		if (minDist == 0)
		{
			minDist = 10000;
			glm::vec3 ray_origin = glm::vec3(position.x, position.y + 4, 0);
			position.y += 4;
			for (int i = 0; i < sortedModels.size(); i++)
			{
				glm::mat4 boxMatrix = sortedModels[i]->getModelMatrix();
				sortedModels[i]->getMinMaxBouding(aabb_min, aabb_max);
				glm::decompose(boxMatrix, boxScale, glm::quat(), glm::vec3(), glm::vec3(), glm::vec4());
				aabb_max *= boxScale*boxScale;
				aabb_min *= boxScale*boxScale;
				if (collision::TestRayOBBIntersection(ray_origin, ray_direction, aabb_min, aabb_max, boxMatrix, intersection_distance))
				{
					if (intersection_distance < minDist)
					{
						minDist = intersection_distance;
					}
				}
			}
		}
		//If the ray is in reach then create attackbox
		if (minDist <= 4.0f && minDist != 0)
		{
			position.y -= minDist;
			Projectile* temp = new Projectile;
			temp->aoe(box, position, direction, 5.0f, scale);
			allProjectiles.push_back(temp);
		}
	}
}
