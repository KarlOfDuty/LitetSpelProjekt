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

glm::vec2 Projectile::getPosition()
{
	return this->position;
}

void Projectile::update(float dt, std::vector<Model*> &allObjects)
{
	//Update only if there is has been no collision
	if (!hasCollided)
	{
		//Check if it should be in use else set it to not being used
		if (timeSinceCollision.getElapsedTime().asSeconds() < 10)
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
}

void Projectile::draw(Shader shader)
{
	if (isUsed)
	{
		glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, &model->getModelMatrix()[0][0]);
		model->draw(shader);
	}
}

void Projectile::shoot(Model* projectileModel, glm::vec2 startPos, glm::vec2 projectileDirection, glm::vec2 projectileRetardation, float projectileVelocity, glm::vec3 projectileScale, bool shouldRotate)
{
	//Copy info supplied
	position = startPos;
	scale = projectileScale;
	direction = projectileDirection;
	retardation = projectileRetardation;

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
	timeSinceCollision.restart();
}

void Projectile::collision(std::vector<Model*> &allObjects)
{
	//Calculate closest model to check collision for
	std::vector<glm::vec2> arrowPoints = model->getPoints();
	int index = -1;
	float minDistance = 1000;
	for (int i = 0; i < allObjects.size(); i++)
	{
		float distance = glm::length(position - glm::vec2(allObjects[i]->getModelMatrix()[3]));
		if (distance < minDistance)
		{
			minDistance = distance;
			index = i;
		}
	}

	//If something was in range check collision
	if (index != -1)
	{
		std::vector<glm::vec2> objectPoints = allObjects[index]->getPoints();
		glm::vec2 mtv;
		if (collision::collision(arrowPoints, objectPoints, mtv))
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
	}
}