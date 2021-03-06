#include "PlayerShark.h"


PlayerShark::PlayerShark(Model* model, bool inWater) :PlayerChar(model, inWater)
{
	this->maxJumps = 1;
	this->jumpHeight = 210;
	this->arrowModel = new Model();
	arrowModel = new Model();
	arrowModel->readModel("models/Characters/Shark/arrow.bb");
	arrowModel->setupModel();
	this->arrowVelocity = 300.0f;
}

PlayerShark::~PlayerShark()
{

}

void PlayerShark::operator=(const PlayerShark &originalObject)
{
	PlayerChar::operator=(originalObject);
}

int PlayerShark::getMaxJumps()
{
	return maxJumps;
}

float PlayerShark::getJumpHeight()
{
	return jumpHeight;
}

void PlayerShark::shootArrow(std::vector<Projectile*>& allProjectiles, glm::vec2 position, glm::vec2 direction)
{
	if (attackCooldown.getElapsedTime().asSeconds() > 0.5f)
	{
		//Check how many arrows are active in the arrow vector
		int activeArrows = 0;
		for (int i = 0; i < allProjectiles.size(); i++)
		{
			if (allProjectiles[i]->isInUse())
				activeArrows++;
		}

		glm::vec3 scale(3.0f, -3.0f, 3.0f);

		//Reuse old arrow if possible otherwize create a new
		if (activeArrows < allProjectiles.size())
		{
			for (int i = 0; i < allProjectiles.size(); i++)
			{
				if (!allProjectiles[i]->isInUse())
				{
					allProjectiles[i]->shoot(arrowModel, 1, position, direction, glm::vec2(50.0f, 300.0f), arrowVelocity, scale);
					i = (int)allProjectiles.size();
				}
			}
		}
		else
		{
			Projectile* temp = new Projectile();
			temp->shoot(arrowModel, 1, position, direction, glm::vec2(50.0f, 300.0f), arrowVelocity, scale);
			allProjectiles.push_back(temp);
		}
		attackCooldown.restart();
	}
}