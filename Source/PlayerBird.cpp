#include "PlayerBird.h"

PlayerBird::PlayerBird(int HP) :PlayerChar(HP)
{
	Model birdModel1 = Model("", {
		0.1, 0.0, 0.0, 0.0,
		0.0, 0.1, 0.0, 0.0,
		0.0, 0.0, 0.1, 0.0,
		2.0, -0.12, 2.0, 1.0 });

	this->birdModel = Model(birdModel1);
}

PlayerBird::~PlayerBird()
{

}

PlayerBird::PlayerBird(const PlayerBird & originalObject)
{
	this->birdModel = originalObject.birdModel;
}

void PlayerBird::operator=(const PlayerBird &originalObject)
{
	PlayerChar::operator=(originalObject);
}

bool PlayerBird::jumpAllowed()
{
	return false;
}

Model PlayerBird::getModel()const
{
	return this->birdModel.getModelMatrix()[0][0];
}

void PlayerBird::jump()
{
}

void PlayerBird::moveLeft()
{

	birdModel.setModelMatrix({
		0.1, 0.0, 0.0, 0.0,
		0.0, 0.1, 0.0, 0.0,
		0.0, 0.0, 0.1, 0.0,
		2.0 + 0.5, -0.12, 2.0, 1.0 })
}

void PlayerBird::moveRight()
{
}

void PlayerBird::attack()
{
}
