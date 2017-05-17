#include "Enemy.h"
#include "Player.h"
#include "Trigger.h"

Enemy::Enemy()
{
	this->health = 10;
	this->damage = 1;
}


Enemy::Enemy(int health, Model* model, int damage, glm::vec3 enemyStartPos, glm::vec3 scaleFactor)
{
	this->health = health;
	this->model = model;
	this->damage = damage;
	this->pos = enemyStartPos;
	this->scaleFactor = scaleFactor;
	setPos(pos);
	isOnGround = true;
	playerSeen = false;
	angle = 0;
	this->checkPoint.x = enemyStartPos.x;
	bossImmunity = false;
}

Enemy::~Enemy()
{

}

void Enemy::setPos(glm::vec3 position)
{
	pos = position;
	this->model->setModelMatrix(glm::mat4(
		scaleFactor.x, 0.0, 0.0, 0.0,
		0.0, scaleFactor.y, 0.0, 0.0,
		0.0, 0.0, scaleFactor.z, 0.0,
		pos.x, pos.y, pos.z, 1.0
	));
}

void Enemy::setHealth(int health)
{
	this->health = health;
}

glm::vec3 Enemy::getPos() const
{
	return pos;
}

std::string Enemy::type() const
{
	return "Enemy";
}

std::vector<glm::vec2> Enemy::getPoints()
{
	return model->getPoints();
}

int Enemy::getDamage() const
{
	return damage;
}

int Enemy::getHealth() const
{
	return this->health;
}
Model* Enemy::getModel()
{
	return this->model;
}

void Enemy::rotateModel(float direction)
{
	model->setRotationMatrix(glm::rotate(glm::mat4(), glm::radians(direction), glm::vec3(0.0f, 1.0f, 0.0f)));
	model->rotate();
}

void Enemy::applyDamage(int appliedDamage)
{
	if (!bossImmunity)
	{
		if (this->damageImmunity.getElapsedTime().asSeconds() >= 0.5)
		{
			this->health -= appliedDamage;
			this->damageImmunity.restart();
		}
	}
}

void Enemy::groundCheck()
{
	groundPos = 0.0f;

	if (pos.y > groundPos)
	{
		isOnGround = false;
	}
}

void Enemy::setBossImmunity(bool isImmune)
{
	this->bossImmunity = isImmune;
}

bool Enemy::collision(std::vector<Model*> &allModels)
{
	int index = -1;
	float minDist = 1000;
	for (int i = 0; i < allModels.size(); i++)
	{
		float distance = glm::length(pos - glm::vec3(allModels[i]->getModelMatrix()[3]));
		if (minDist > distance)
		{
			minDist = distance;
			index = i;
		}
	}
	if (index != -1)
	{
		std::vector<glm::vec2> enemyPoints = this->getModel()->getPoints();
		std::vector<glm::vec2> objectPoints = allModels[index]->getPoints();
		glm::vec2 mtv;
		if (collision::collision(enemyPoints, objectPoints, mtv))
		{	
			//Get rotation
			glm::quat rotation;
			glm::decompose(allModels[index]->getModelMatrix(),glm::vec3(),rotation,glm::vec3(),glm::vec3(),glm::vec4());
			//Convert from quat to radians
			double t3 = +2.0 * (rotation.w * rotation.z + rotation.x * rotation.y);
			double t4 = +1.0 - 2.0f * ((rotation.y * rotation.y) + rotation.z * rotation.z);
			radians = (float)-std::atan2(t3, t4);
			if (radians > 0.0f && radians < 0.79f)
			{
				pos.y += mtv.y;
				mtv.x = 0;
			}
			else
			{
				pos.x += mtv.x;
				pos.y += mtv.y;
			}

			if (mtv.y > 0)
			{
				if (pos.y < 0) pos.y = 0;
				groundPos = pos.y;
			}
			collidedFrom = mtv;

			setPos(pos);
			return true;
		}
		else
		{
			collidedFrom = glm::vec2(0,0);
		}
	}
	return false;
}

bool Enemy::collisionWithPlayer(Player* player)
{

	std::vector<glm::vec2> enemyPoints = this->getModel()->getPoints();
	if (collision::collision(enemyPoints, player->getPoints()))
	{
		player->applyDamage(this->getDamage());
		return true;
	}
	return false;
}

void Enemy::update(float dt, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels, Player* player)
{
	if (glm::length(pos - player->getPos()) < 25.0f)
	{
		updateThis(dt, pos, checkPoint, allSmallBats, allModels, player);
	}
}

void Enemy::draw(Shader shader)
{
	model->draw(shader);
}
