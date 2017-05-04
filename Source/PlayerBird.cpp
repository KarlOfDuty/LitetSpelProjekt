#include "PlayerBird.h"

PlayerBird::PlayerBird(int health, Model model, bool inWater) :PlayerChar(health, model, inWater)
{
	this->maxJumps = 2;
	this->jumpHeight = 15;
	this->arrowModel = new Model("models/cube/cubeGreen.obj");
	this->arrowVelocity = 30.0f;
}

PlayerBird::~PlayerBird()
{

}
void PlayerBird::operator=(const PlayerBird &originalObject)
{
	PlayerChar::operator=(originalObject);
}

int PlayerBird::getMaxJumps()
{
	return maxJumps;
}

float PlayerBird::getJumpHeight()
{
	return jumpHeight;
}

void PlayerBird::waterEffect()
{
	this->setHealth(0);
}

void PlayerBird::shootArrow(std::vector<Projectile*>& allProjectiles, glm::vec2 position, glm::vec2 direction) const
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