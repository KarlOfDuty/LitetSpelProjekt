#include "PlayerBird.h"

PlayerBird::PlayerBird(Model model, bool inWater) :PlayerChar(model, inWater)
{
	this->maxJumps = 2;
	this->jumpHeight = 210;
	box = new Model("models/cube/cubeGreen.obj");
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

void PlayerBird::meleeAttack(std::vector<Projectile*> &allAttackBoxes, glm::vec2 position, glm::vec2 direction, float velocity)
{
	if (attackCooldown.getElapsedTime().asSeconds() > 0.5f)
	{
		glm::vec3 scale(0, 35.0f, 1.0f);
		position.y += 15.f;
		Projectile* temp = new Projectile;
		temp->melee(box, position, direction, velocity, scale);
		allAttackBoxes.push_back(temp);
		attackCooldown.restart();
	}
}
