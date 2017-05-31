#include "Projectile.h"

std::vector<glm::vec2> Projectile::getPoints()
{
	return model->getPoints();
}

glm::vec3 Projectile::getPos() const
{
	return model->getPos();
}

std::string Projectile::type() const
{
	return "Projectile";
}

Projectile::Projectile()
{
	hasCollided = false;
	isUsed = true;
}

Projectile::~Projectile()
{

}

bool Projectile::isInUse()
{
	return isUsed;
}

bool Projectile::isProjectileAttack()
{
	return isProjectile;
}

bool Projectile::isAoeAttack()
{
	return isAoe;
}

bool Projectile::isMeleeAttack()
{
	return isMelee;
}

bool Projectile::isCollidingWithWorld()
{
	return hasCollided;
}

void Projectile::disableArrow()
{
	isUsed = false;
}

glm::vec2 Projectile::getPosition()
{
	return this->position;
}

void Projectile::update(float dt, std::vector<Model*> &allObjects, glm::vec2 playerPos, bool goingRight)
{
	//Update only if there is has been no collision
	if (!hasCollided)
	{
		//Check if it should be in use else set it to not being used
		if (timeSinceCollision.getElapsedTime().asSeconds() < 10)
		{
			if (!isAoe && !isMelee)
			{
				//Retardation
				velocity.x -= retardation.x*dt;
				if (velocity.x < 0) velocity.x = 0;
				velocity.y -= retardation.y*dt;

				//Set new position
				position.x += direction.x*velocity.x*dt;
				position.y += velocity.y*dt;
				model->setModelMatrix({
					1.0, 0.0, 0.0, 0.0,
					0.0, 1.0, 0.0, 0.0,
					0.0, 0.0, 1.0, 0.0,
					position.x, position.y , 0.0, 1.0
				});

				//Set rotation if isRotating, else just set scale
				glm::mat4 scaleMat = glm::scale(glm::mat4(), scale);
				if (isRotating)
				{
					rotation = -atan2(velocity.x*direction.x, velocity.y);

					glm::mat4 rotMat = glm::rotate(glm::mat4(), rotation, glm::vec3(0.0f, 0.0f, 1.0f));

					model->setRotationMatrix(rotMat*scaleMat);

					model->rotate();
				}
				else
				{
					model->setRotationMatrix(scaleMat);

					model->rotate();
				}

				//Check the collision and fix if its colliding
				collision(allObjects);
			}
			else if (isAoe)
			{
				scale.x += direction.x * velocity.x * dt;
				scale.y += direction.y * velocity.y * dt;
				position.x += direction.x * velocity.x/2 * dt;
				position.y += direction.y * velocity.y/2 * dt;

				model->setModelMatrix({
					1.0, 0.0, 0.0, 0.0,
					0.0, 1.0, 0.0, 0.0,
					0.0, 0.0, 1.0, 0.0,
					position.x, position.y , 0.0, 1.0
				});

				//Set rotation if isRotating, else just set scale
				glm::mat4 scaleMat = glm::scale(glm::mat4(), scale);
				model->setRotationMatrix(scaleMat);
				model->rotate();

				if (abs(scale.x*direction.x) >= 20)
				{
					isUsed = false;
				}
				if (scale.y*direction.y >= 20)
				{
					isUsed = false;
				}
			}
			else
			{
				if (!enemyMeleeAttack)
				{
					if (goingRight)
					{
						if (direction.x != 1)
						{
							direction.x = 1;
							scale.x = -scale.x;
						}
					}
					else
					{
						if (direction.x != -1)
						{
							direction.x = -1;
							scale.x = -scale.x;
						}
					}

					scale.x += direction.x * velocity.x * dt;
					scale.y += direction.y * velocity.y * dt;
					position.x = playerPos.x + scale.x / 2 + 10.0f*direction.x;
					position.y = playerPos.y + 20.f;
					model->setModelMatrix({
						1.0, 0.0, 0.0, 0.0,
						0.0, 1.0, 0.0, 0.0,
						0.0, 0.0, 1.0, 0.0,
						position.x, position.y , 0.0, 1.0
					});

					//Set rotation if isRotating, else just set scale
					glm::mat4 scaleMat = glm::scale(glm::mat4(), scale);
					model->setRotationMatrix(scaleMat);
					model->rotate();

					if (abs(scale.x*direction.x) >= 20)
					{
						isUsed = false;
					}
					if (scale.y*direction.y >= 20)
					{
						isUsed = false;
					}
				}
				else
				{
					position.x += (velocity.x/2) * direction.x * dt;
					model->setModelMatrix({
						1.0, 0.0, 0.0, 0.0,
						0.0, 1.0, 0.0, 0.0,
						0.0, 0.0, 1.0, 0.0,
						position.x, position.y , 0.0, 1.0
					});
					scale.x += velocity.x * dt;
					//scale.y += direction.y * velocity.y * dt;
					glm::mat4 scaleMat = glm::scale(glm::mat4(), scale);
					model->setRotationMatrix(scaleMat);
					model->rotate();

					if (abs(scale.x*direction.x) >= 35)
					{
						isUsed = false;
					}
					if (scale.y*direction.y >= 75)
					{
						isUsed = false;
					}
				}
			}
		}
		else
		{
			isUsed = false;
		}
		
	}
	else
	{
		//If it has collided and the time has elapsed, set it to not being used
		if (timeSinceCollision.getElapsedTime().asSeconds() > 5.0f)
		{
			isUsed = false;
		}
	}
	std::vector<glm::vec2> allPoints = model->getPoints();
}

void Projectile::draw(Shader shader)
{
	if (isUsed)
	{
		glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, &model->getModelMatrix()[0][0]);
		model->draw(shader);
	}
}

void Projectile::shoot(Model* projectileModel, glm::vec2 startPos, glm::vec2 projectileDirection, glm::vec2 projectileRetardation, float projectileVelocity, glm::vec3 projectileScale, bool shouldRotate, bool shouldDelete)
{
	//Copy info supplied
	position = startPos;
	scale = projectileScale;
	direction = projectileDirection;
	retardation = projectileRetardation;
	deleteOnImpact = shouldDelete;
	enemyMeleeAttack = false;
	//Create new modelmat for model
	glm::mat4 modelMat({
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		position.x, position.y, 0.0, 1.0
	});
	model = new Model(projectileModel, modelMat);

	//Set the velocity
	velocity = glm::vec2(glm::abs(direction.x*projectileVelocity), direction.y*projectileVelocity);
	
	//If it should rotate, calculate rotation ELSE just set scale
	glm::mat4 scaleMat = glm::scale(glm::mat4(), scale);
	if (shouldRotate)
	{
		rotation = -atan2(velocity.x*direction.x, velocity.y);

		glm::mat4 rotMat = glm::rotate(glm::mat4(), rotation, glm::vec3(0.0f, 0.0f, 1.0f));

		model->setRotationMatrix(rotMat*scaleMat);
		model->rotate();

		isRotating = true;
	}
	else
	{
		model->setRotationMatrix(scaleMat);
		model->rotate();
	}

	//Reset booleans and clock
	hasCollided = false;
	isUsed = true;
	isProjectile = true;
	isAoe = false;
	isMelee = false;
	timeSinceCollision.restart();
}

void Projectile::aoe(Model* projectileModel, glm::vec2 startPos, glm::vec2 projectileDirection, float projectileVelocity, glm::vec3 projectileScale)
{
	//Copy info supplied
	position = startPos;
	scale = projectileScale;
	direction = projectileDirection;
	deleteOnImpact = false;
	enemyMeleeAttack = false;

	//Create new modelmat for model
	glm::mat4 modelMat({
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		position.x, position.y, 0.0, 1.0
	});
	if (model == nullptr)
	{
		model = new Model(projectileModel, modelMat);
	}
	else
	{
		model->setModelMatrix(modelMat);
	}

	//Set the velocity
	velocity = glm::vec2(glm::abs(direction.x*projectileVelocity), direction.y*projectileVelocity);

	glm::mat4 scaleMat = glm::scale(glm::mat4(), scale);
	model->setRotationMatrix(scaleMat);
	model->rotate();

	//Reset booleans and clock
	isRotating = false;
	hasCollided = false;
	isUsed = true;
	isProjectile = false;
	isAoe = true;
	isMelee = false;
	timeSinceCollision.restart();
}

void Projectile::melee(Model * projectileModel, glm::vec2 startPos, glm::vec2 projectileDirection, float projectileVelocity, glm::vec3 projectileScale)
{
	//Copy info supplied
	position = startPos;
	scale = projectileScale;
	direction = projectileDirection;
	deleteOnImpact = false;
	enemyMeleeAttack = false;

	//Create new modelmat for model
	glm::mat4 modelMat({
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		position.x, position.y, 0.0, 1.0
	});
	if (model == nullptr)
	{
		model = new Model(projectileModel, modelMat);
	}
	else
	{
		model->setModelMatrix(modelMat);
	}

	//Set the velocity
	velocity = glm::vec2(glm::abs(direction.x*projectileVelocity), direction.y*projectileVelocity);

	glm::mat4 scaleMat = glm::scale(glm::mat4(), scale);
	model->setRotationMatrix(scaleMat);
	model->rotate();

	//Reset booleans and clock
	isRotating = false;
	hasCollided = false;
	isUsed = true;
	isProjectile = false;
	isAoe = false;
	isMelee = true;
	timeSinceCollision.restart();
}

void Projectile::enemyMelee(Model * projectileModel, glm::vec2 startPos, glm::vec2 projectileDirection, float projectileVelocity, glm::vec3 projectileScale)
{
	//Copy info supplied
	position = startPos;
	scale = projectileScale;
	direction = projectileDirection;
	deleteOnImpact = false;
	enemyMeleeAttack = true;
	//Create new modelmat for model
	glm::mat4 modelMat({
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		position.x, position.y, 0.0, 1.0
	});
	if (model == nullptr)
	{
		model = new Model(projectileModel, modelMat);
	}
	else
	{
		model->setModelMatrix(modelMat);
	}

	//Set the velocity
	velocity = glm::vec2(glm::abs(direction.x*projectileVelocity), direction.y*projectileVelocity);

	glm::mat4 scaleMat = glm::scale(glm::mat4(), scale);
	model->setRotationMatrix(scaleMat);
	model->rotate();

	//Reset booleans and clock
	isRotating = false;
	hasCollided = false;
	isUsed = true;
	isProjectile = false;
	isAoe = false;
	isMelee = true;
	timeSinceCollision.restart();
}

void Projectile::collision(std::vector<Model*> &allObjects)
{
	std::vector<Model*> closeObjects;
	for (int i = 0; i < allObjects.size(); i++)
	{
		glm::vec3 objectMin, objectMax;
		allObjects[i]->getScaledMinMaxBouding(objectMin, objectMax);
		glm::vec2 distance = allObjects[i]->getPos() - getPos();
		if (abs(distance.x) < 50.0f + objectMax.x)
		{
			if (abs(distance.y) < 50.0f + objectMax.y)
			{
				closeObjects.push_back(allObjects[i]);
			}
		}
	}

	std::cout << closeObjects.size();

	if (!closeObjects.empty())
	{
		std::vector<glm::vec2> projectilePoints = getPoints();

		for (int i = 0; i < closeObjects.size(); i++)
		{
			std::vector<glm::vec2> objectPoints = closeObjects[i]->getPoints();
			glm::vec2 mtv;
			if (collision::collision(projectilePoints, objectPoints, mtv))
			{
				if (!deleteOnImpact)
				{
					position += mtv;
					model->setModelMatrix({
						1.0, 0.0, 0.0, 0.0,
						0.0, 1.0, 0.0, 0.0,
						0.0, 0.0, 1.0, 0.0,
						position.x, position.y , 0.0, 1.0
					});
					model->rotate();
					velocity = glm::vec2(0);
					hasCollided = true;
					timeSinceCollision.restart();
				}
				else
				{
					isUsed = false;
				}
			}
		}
	}
}