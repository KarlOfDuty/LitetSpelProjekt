#include "PlayerShark.h"


PlayerShark::PlayerShark(int health, Model model, bool inWater) :PlayerChar(health, model, inWater)
{
	this->maxJumps = 1;
	this->jumpHeight = 10;
	this->arrowModel = new Model("models/cube/cubeGreen.obj");
	this->arrowVelocity = 30.0f;
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
void PlayerShark::waterEffect()
{
	this->setDiving(true);
}
void PlayerShark::dive()
{
	setDiving(true);
}
void PlayerShark::shootArrow(std::vector<Projectile*>& allProjectiles, glm::vec2 position, glm::vec2 direction) const
{
	//Check how many arrows are active in the arrow vector
	int activeArrows = 0;
	for (int i = 0; i < allProjectiles.size(); i++)
	{
		if (allProjectiles[i]->isInUse())
			activeArrows++;
	}

	glm::vec3 scale(0.1f, 1.0f, 0.1f);

	//Reuse old arrow if possible otherwize create a new
	if (activeArrows < allProjectiles.size())
	{
		for (int i = 0; i < allProjectiles.size(); i++)
		{
			if (!allProjectiles[i]->isInUse())
			{
				allProjectiles[i]->shoot(arrowModel, position, direction, glm::vec2(5.0f, 30.0f), arrowVelocity, scale);
				i = (int)allProjectiles.size();
			}
		}
	}
	else
	{
		Projectile* temp = new Projectile();
		temp->shoot(arrowModel, position, direction, glm::vec2(5.0f, 30.0f), arrowVelocity, scale);
		allProjectiles.push_back(temp);
	}
}