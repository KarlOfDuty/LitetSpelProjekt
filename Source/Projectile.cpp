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

void Projectile::update(float dt,std::vector<Model*> &allObjects)
{
	if (!hasCollided)
	{
		if (timeSinceCollision.getElapsedTime().asSeconds() < 10)
		{
			velocity.x -= 5.0*dt;
			if (velocity.x < 0) velocity.x = 0;

			velocity.y -= 40.0*dt;

			position.x += direction.x*velocity.x*dt;
			position.y += velocity.y*dt;

			model->setModelMatrix({
				1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				position.x, position.y , 0.0, 1.0
			});

			rotation = -atan2(velocity.x*direction.x, velocity.y);

			glm::mat4 rotMat = glm::rotate(glm::mat4(), rotation, glm::vec3(0.0f, 0.0f, 1.0f));
			glm::mat4 scaleMat = glm::scale(glm::mat4(), glm::vec3(0.1, 1.0, 0.1));
			
			model->setRotationMatrix(rotMat*scaleMat);
			
			model->rotate();

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
		else
		{
			isUsed = false;
		}
		
	}
	else
	{
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

void Projectile::shoot(sf::Window &window ,glm::vec2 startPos, Model* arrow)
{
	position = startPos;
	glm::mat4 modelMat({
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		position.x, position.y, 0.0, 1.0
	});
	if (model == nullptr)
	{
		model = new Model(arrow, modelMat);
	}
	
	glm::vec2 mousePos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
	glm::vec2 middleScreen(window.getSize().x / 2, window.getSize().y / 2);
	rotation = atan2(mousePos.x - middleScreen.x, mousePos.y - middleScreen.y);

	glm::mat4 rotMat = glm::rotate(glm::mat4(), rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scaleMat = glm::scale(glm::mat4(), glm::vec3(0.1, 1.0, 0.1));

	model->setRotationMatrix(rotMat*scaleMat);
	model->rotate();
	
	direction = glm::normalize(glm::vec2(sin(rotation), -cos(rotation)));
	float startVelocity = 30.0f;

	velocity = glm::vec2(glm::abs(direction.x*startVelocity), direction.y*startVelocity);
	hasCollided = false;
	isUsed = true;
	timeSinceCollision.restart();
}