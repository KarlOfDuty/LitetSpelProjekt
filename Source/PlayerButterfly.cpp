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
	if (attackCooldown.getElapsedTime().asSeconds() > 0.5f)
	{
		glm::vec2 direction(0, 1);
		glm::vec3 scale(4.0f, 0.1f, 1.0f);

		glm::vec3 ray_origin = glm::vec3(position.x, position.y + 0.001, 0);
		glm::vec3 ray_direction(0, -1, 0);
		glm::vec3 boxScale;
		glm::vec3 aabb_min;
		glm::vec3 aabb_max;
		float intersection_distance;
		for (int i = 0; i < allStaticModels.size(); i++)
		{
			glm::mat4 boxMatrix = allStaticModels[i]->getModelMatrix();
			allStaticModels[i]->getMinMaxBouding(aabb_min, aabb_max);
			glm::decompose(boxMatrix, boxScale, glm::quat(), glm::vec3(), glm::vec3(), glm::vec4());
			aabb_max *= boxScale;
			aabb_min *= boxScale;
			if (collision::TestRayOBBIntersection(ray_origin, ray_direction, aabb_min, aabb_max, boxMatrix, intersection_distance))
			{
				if (intersection_distance == 0)
				{
					glm::vec3 newRay = ray_origin;
					newRay.y += 4;
					if (collision::TestRayOBBIntersection(newRay, ray_direction, aabb_min, aabb_max, boxMatrix, intersection_distance))
					{
						position.y += 4.0f - intersection_distance;
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
			temp->aoe(box, position, direction, 5.0f, scale);
			allProjectiles.push_back(temp);
		}
		attackCooldown.restart();
	}
}
