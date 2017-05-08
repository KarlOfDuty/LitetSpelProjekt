#include "PlayerBird.h"

PlayerBird::PlayerBird(int health, Model model, bool inWater) :PlayerChar(health, model, inWater)
{
	this->maxJumps = 2;
	this->jumpHeight = 15;
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

void PlayerBird::waterEffect()
{
	this->setHealth(0);
}

void PlayerBird::meleeAttack(std::vector<Projectile*> &allAttackBoxes, glm::vec2 position, glm::vec2 direction, float velocity)
{
	glm::vec3 scale(0, 3.0f, 1.0f);
	position.y += 1.5f;
	Projectile* temp = new Projectile;
	temp->melee(box, position, direction, velocity, scale);
	allAttackBoxes.push_back(temp);
}
