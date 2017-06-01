#include "Enemy.h"
#include "Player.h"
#include "Trigger.h"

Enemy::Enemy()
{
	this->health = 10;
	this->damage = 1;
}


Enemy::Enemy(int health, Model* model, int damage, int immunityTime, glm::vec3 enemyStartPos, glm::vec3 scaleFactor, SoundSystem * sound)
{
	this->health = health;
	this->model = model;
	this->damage = damage;
	this->immunityTime = immunityTime;
	this->pos = enemyStartPos;
	this->scaleFactor = scaleFactor;
	setPos(pos);
	isOnGround = true;
	playerSeen = false;
	angle = 0;
	this->checkPoint.x = enemyStartPos.x;
	bossImmunity = false;
	this->sound = sound;
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
		if (this->damageImmunity.getElapsedTime().asSeconds() >= immunityTime)
		{
			this->playerSeen = true;
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
	std::vector<Model*> closeObjects;
	for (int i = 0; i < allModels.size(); i++)
	{
		glm::vec3 objectMin, objectMax;
		allModels[i]->getScaledMinMaxBouding(objectMin, objectMax);
		glm::vec2 distance = allModels[i]->getPos() - getPos();
		if (abs(distance.x) < 100.0f + objectMax.x)
		{
			if (abs(distance.y) < 100.0f + objectMax.y)
			{
				closeObjects.push_back(allModels[i]);
			}
		}
	}
	collidedFrom = glm::vec2(0,0);
	collisionNormal = glm::vec2(0, 0);
	bool hasCollided = false;
	for (int i = 0; i < closeObjects.size(); i++)
	{
		std::vector<glm::vec2> enemyPoints = this->getModel()->getPoints();
		std::vector<glm::vec2> objectPoints = closeObjects[i]->getPoints();
		glm::vec2 mtv;
		glm::vec2 normal;
		if (collision::collision(enemyPoints, objectPoints, mtv, normal))
		{
			//Get rotation
			glm::quat rotation;
			glm::decompose(closeObjects[i]->getModelMatrix(), glm::vec3(), rotation, glm::vec3(), glm::vec3(), glm::vec4());
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
				if (normal.y < 0.707)
				{
					pos.x += mtv.x;
				}
				pos.y += mtv.y;
			}

			if (mtv.y > 0)
			{
				if (pos.y < 0) pos.y = 0;
				groundPos = pos.y;
			}
			if (abs(collisionNormal.x) < abs(normal.x))
			{
				collisionNormal = normal;
			}
			collidedFrom += mtv;
			setPos(pos);
			hasCollided = true;
		}
	}
	return hasCollided; 
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

bool Enemy::getBossKill() const
{
	return bossKill;
}

void Enemy::update(float dt, std::vector<Enemy*> allSmallBats, std::vector<Model*> &allModels, Player* player)
{
	if (glm::length(pos - player->getPos()) < 400.0f)
	{
		updateThis(dt, pos, checkPoint, allSmallBats, allModels, player);
	}
}

void Enemy::draw(Shader shader)
{
	model->draw(shader);
}
