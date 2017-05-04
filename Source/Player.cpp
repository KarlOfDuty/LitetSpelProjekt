#include "Player.h"
#include <glm\gtx\matrix_decompose.hpp>
const double PI = 3.14159265358979323846;

void Player::freeMemory()
{
	for (int i = 0; i < 3; i++)
	{
		delete this->playerCharacters[i];
	}
}

Player::Player()
{
	Model* birdModel = new Model("models/Characters/Bird/BirdTest1.obj", modelMatrix);
	Model* sharkModel = new Model("models/sphere/sphere.obj", modelMatrix);
	Model* butterflyModel = new Model("models/cube/cubeGreen.obj", modelMatrix);

	arrow = new Model("models/cube/cubeGreen.obj");

	this->modelMatrix[3] = glm::vec4(0.0f, 2.0f, 0.0f, 1.0);
	this->modelMatrix *= glm::rotate(glm::mat4(), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	this->modelMatrix *= glm::scale(glm::vec3(0.08f, 0.08f, 0.08f));

	angle = 0;
	this->movementSpeed = 4.0f;
	//Add characters
	this->playerCharacters[0] = new PlayerBird(10000, birdModel, false);
	this->playerCharacters[1] = new PlayerShark(10000, sharkModel, false);
	this->playerCharacters[2] = new PlayerButterfly(10000, butterflyModel, false);
	this->player = playerCharacters[0];
	this->isOnGround = true;

	arrows = std::vector<Projectile*>();
}

Player::~Player()
{
	this->freeMemory();
}

PlayerChar* Player::getCurrentCharacter()
{
	return player;
}

int Player::getDamage() const
{
	return playerCharacters[0]->getDamage();
}

std::vector<glm::vec2> Player::getPoints()
{
	std::vector<glm::vec2> playerPoints;
	playerPoints.push_back(glm::vec2(-0.5f, -0.0f));
	playerPoints.push_back(glm::vec2(0.5f, -0.0f));
	playerPoints.push_back(glm::vec2(0.5f, 1.0f));
	playerPoints.push_back(glm::vec2(-0.5f, 1.0f));
	for (int k = 0; k < playerPoints.size(); k++)
	{
		playerPoints[k] += glm::vec2(getPos());
	}
	return playerPoints;
}
//Swaps the current player
void Player::swap(int character)
{
	player = playerCharacters[character];
}
//Makes the player jump
void Player::jump()
{
	if(player->getDiving() != true)
	{	if (player->getMaxJumps() > jumps)
		{
			velocityY = player->getJumpHeight();
			jumps++;
		}
	}
	else
	{
		if (player->getMaxJumps() > jumps)
		{
			velocityY = player->getJumpHeight();
		}
	}
}
void Player::lightAttackPressed(sf::Window &window)
{
	PlayerButterfly* butterfly = dynamic_cast<PlayerButterfly*>(player);
	if (butterfly)
	{
		int mouseX = sf::Mouse::getPosition(window).x;
		int middleScreenX = window.getSize().x / 2;
		glm::vec2 position = (mouseX >= middleScreenX) ? glm::vec2(getPos().x+3.0f, getPos().y) : glm::vec2(getPos().x-3.0f, getPos().y);
		glm::vec2 direction(0, 1);
		glm::vec3 scale(3.0f,2.0f,1.0f);

		Projectile* temp = new Projectile;
		temp->aoe(arrow, position, direction, glm::vec2(0.0f, 35.0f), 12.0f, scale);
		arrows.push_back(temp);

	}
}
void Player::lightAttackReleased(sf::Window &window)
{
	PlayerBird* bird = dynamic_cast<PlayerBird*>(player);
	if (bird != nullptr && !sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
	{
		//Get direction and scale
		glm::vec2 mousePos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
		glm::vec2 middleScreen(window.getSize().x / 2, window.getSize().y / 2);
		float rotation = atan2(mousePos.x - middleScreen.x, mousePos.y - middleScreen.y);
		glm::vec2 direction = glm::normalize(glm::vec2(sin(rotation), -cos(rotation)));
		glm::vec2 startPos = glm::vec2(getPos().x, getPos().y + 2.0f);
		bird->shootArrow(arrows, startPos, direction);
	}
}
void Player::heavyAttackPressed(sf::Window &window)
{
	PlayerButterfly* butterfly = dynamic_cast<PlayerButterfly*>(player);
	if (butterfly)
	{
		int mouseX = sf::Mouse::getPosition(window).x;
		int middleScreenX = window.getSize().x / 2;
		glm::vec2 position = (mouseX >= middleScreenX) ? glm::vec2(getPos().x + 10.0f, getPos().y) : glm::vec2(getPos().x - 10.0f, getPos().y);
		glm::vec2 direction(0, 1);
		glm::vec3 scale(3.0f, 2.0f, 1.0f);

		Projectile* temp = new Projectile;
		temp->aoe(arrow, position, direction, glm::vec2(0.0f, 35.0f), 12.0f, scale);
		arrows.push_back(temp);

	}
}
void Player::heavyAttackReleased(sf::Window &window)
{
	PlayerBird* bird = dynamic_cast<PlayerBird*>(player);
	if (bird != nullptr)
	{
		//Get direction and scale
		glm::vec2 mousePos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
		glm::vec2 middleScreen(window.getSize().x / 2, window.getSize().y / 2);
		float rotation = atan2(mousePos.x - middleScreen.x, mousePos.y - middleScreen.y);
		glm::vec2 direction = glm::normalize(glm::vec2(sin(rotation), -cos(rotation)));
		glm::vec2 startPos = glm::vec2(getPos().x, getPos().y + 2.0f);
		bird->shootArrow(arrows, startPos, direction);
		bird->arrowVelocity = 30.0f;
	}
}

void Player::aiming(sf::Window &window, float dt)
{
	PlayerBird* bird = dynamic_cast<PlayerBird*>(player);
	if (bird != nullptr)
	{
		if (bird->arrowVelocity >= 60.0f)
		{
			bird->arrowVelocity = 60.0f;
		}
		else
		{
			bird->arrowVelocity += 5.0f * dt;
		}

		glm::vec2 mousePos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
		glm::vec2 middleScreen(window.getSize().x / 2, window.getSize().y / 2);
		float rotation = atan2(mousePos.x - middleScreen.x, mousePos.y - middleScreen.y);
		glm::vec2 direction = glm::normalize(glm::vec2(sin(rotation), -cos(rotation)));

		glm::vec2 position = glm::vec2(getPos().x, getPos().y + 2.f);
		glm::vec2 velocity = glm::vec2(glm::abs(direction.x*bird->arrowVelocity), direction.y*bird->arrowVelocity);
		for (int i = 0; i < 30; i++)
		{
			velocity.x -= 5.0f*0.01;
			if (velocity.x < 0) velocity.x = 0;
			velocity.y -= 40.0f*0.01;
			position.x += direction.x*velocity.x*0.02;
			position.y += velocity.y*0.02;

			glm::mat4 modelMat({
				0.1, 0.0, 0.0, 0.0,
				0.0, 0.1, 0.0, 0.0,
				0.0, 0.0, 0.1, 0.0,
				position.x, position.y , 0.0, 1.0
			});

			if (debugCubes.size() <= i)
			{
				debugCubes.push_back(new Model(arrow, modelMat));
			}
			else
			{
				debugCubes[i]->setModelMatrix(modelMat);
			}
		}
	}
}

//Sets the playerPos variable
void Player::setPos(glm::vec3 playerPos)
{
	this->modelMatrix[3] = glm::vec4(playerPos,1.0);
}
//True if player is dead
bool Player::playerIsDead()
{
	if (playerCharacters[0]->getHealth() <= 0)
	{
		playerCharacters[0]->setHealth(0);

		return true;
	}
	return false;
}
glm::vec3 Player::getPos() const
{
	return modelMatrix[3];
}
std::string Player::type() const
{
	return "Player";
}
//Update function
void Player::update(sf::Window &window, float dt, std::vector<Model*> &allModels, std::vector<Enemy*> allEnemies)
{
	groundPos = 0.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		PlayerButterfly* butterfly = dynamic_cast<PlayerButterfly*>(player);
		if (butterfly)
		{
			int mouseX = sf::Mouse::getPosition(window).x;
			int middleScreenX = window.getSize().x / 2;
			glm::vec2 position = (mouseX >= middleScreenX) ? glm::vec2(getPos().x + 3.0f, getPos().y) : glm::vec2(getPos().x - 3.0f, getPos().y);
			glm::vec2 direction(0, 1);
			glm::vec3 scale(3.0f, 2.0f, 1.0f);

			glm::vec3 ray_origin = glm::vec3(position,0);
			glm::vec3 ray_direction(0, -1, 0);
			glm::vec3 aabb_min(-10, -10, -10);
			glm::vec3 aabb_max(10, 10, 10);
			float intersection_distance = 10000;
			for (int i = 0; i < allModels.size(); i++)
			{
				glm::mat4 boxMatrix = allModels[0]->getModelMatrix();
				if (TestRayOBBIntersection(ray_origin, ray_direction, aabb_min, aabb_max, boxMatrix, intersection_distance))
				{
					std::cout << intersection_distance << std::endl;
					position.y -= intersection_distance - 0.5f;
					i = allModels.size();
				}
			}
			if (intersection_distance != 10000)
			{
				Projectile* temp = new Projectile;
				temp->aoe(arrow, position, direction, glm::vec2(0.0f, 35.0f), 12.0f, scale);
				arrows.push_back(temp);
			}

		}
	}

	for (int i = 0; i < arrows.size(); i++)
	{
		if (arrows[i]->isInUse())
		{
			if (glm::distance(getPos(), arrows[i]->getPos()) < 40.0f)
			{
				arrows[i]->update(dt, allModels);
				std::vector<glm::vec2> arrowPoints = arrows[i]->getPoints();
				for (int k = 0; k < allEnemies.size(); k++)
				{
					if (glm::distance(arrows[i]->getPos(), allEnemies[k]->getPos()) < 2.0f)
					{
						if (collision::collision(arrowPoints, allEnemies[k]->getPoints()))
						{
							arrows[i]->disableArrow();
							allEnemies[k]->applyDamage(100);
							k = allEnemies.size();
						}
					}
				}
			}
			else
			{
				arrows[i]->disableArrow();
			}
		}
	}
	
	//Check if aiming
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		aiming(window, dt);
	}

	if (getPos().y > groundPos && isOnGround)
	{
		isOnGround = false;
	}
	int controller = CONTROLLER0;
	//Move
	if(player->getDiving() != true)
	{
		if (sf::Joystick::getAxisPosition(controller, sf::Joystick::X) < -20)
		{
			velocityX = -movementSpeed*dt;
		}
		else if (sf::Joystick::getAxisPosition(controller, sf::Joystick::X) > 20)
		{
			velocityX = movementSpeed*dt;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			velocityX = -movementSpeed*dt;
			goingLeft = true;
			goingRight = false;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			velocityX = movementSpeed*dt;
			goingRight = true;
			goingLeft = false;
		}

		if (goingLeft == true)
		{
			if (angle != 180)
			{
				this->modelMatrix *= glm::rotate(glm::mat4(), glm::radians(-12.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				angle += 12;
			}
		
		}
		if (goingRight == true)
		{
			if (angle > 0)
			{
				this->modelMatrix  *= glm::rotate(glm::mat4(), glm::radians(12.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				angle -= 12;
			}
		
		}
		//If in air
		if (!isOnGround)
		{
			velocityY -= 30 * dt;
		}

		//Maximum falling speed
		if (velocityY < -30)
		{
			velocityY = -30;
		}
	}
	else 
	{
		if (sf::Joystick::getAxisPosition(controller, sf::Joystick::X) < -20)
		{
			velocityX = -movementSpeed*dt/2;
		}
		else if (sf::Joystick::getAxisPosition(controller, sf::Joystick::X) > 20)
		{
			velocityX = movementSpeed*dt/2;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			velocityX = -movementSpeed*dt/2;
			goingLeft = true;
			goingRight = false;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			velocityX = movementSpeed*dt/2;
			goingRight = true;
			goingLeft = false;
		}
		if (goingLeft == true)
		{
			if (angle != 180)
			{
				this->modelMatrix *= glm::rotate(glm::mat4(), glm::radians(-12.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				angle += 12;
			}

		}

		if (goingRight == true)
		{
			if (angle > 0)
			{
				this->modelMatrix *= glm::rotate(glm::mat4(), glm::radians(12.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				angle -= 12;
			}

		}
		//If in air
		if (!isOnGround)
		{
			velocityY -= 30 * dt * 2;
		}

		//Maximum falling speed
		if (velocityY < -5)
		{
			velocityY = -5;
		}
	}

	if (playerCharacters[2] == player)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && tpCooldown.getElapsedTime().asSeconds() >= 5.0)
		{
			if (goingLeft == true)
			{
				glm::vec3 minus4 = {-4,0,0};
				this->setPos(this->getPos() + minus4);
			}
			else
			{
				glm::vec3 plus4 = {4,0,0};
				this->setPos(this->getPos() + plus4);
			}
			tpCooldown.restart();
		}
	}

	//Apply velocity
	modelMatrix[3].x += velocityX;
	velocityX = 0;
	modelMatrix[3].y += velocityY*dt;
	
	collision(allModels);

	//Handle collision detection with ground
	if (getPos().y <= groundPos && !isOnGround)
	{
		jumps = 0;
		if (velocityY < 0)
		{
			modelMatrix[3].y = groundPos;
			velocityY = 0;
		}
		isOnGround = true;
	}
}
//Draws the models involved
void Player::draw(Shader shader)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, &modelMatrix[0][0]);
	player->draw(shader);

	for (int i = 0; i < arrows.size(); i++)
	{
		if (arrows[i]->isInUse())
		{
			arrows[i]->draw(shader);
		}
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && player == playerCharacters[0])
	{
		for (int i = 0; i < debugCubes.size(); i++)
		{
			glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, &debugCubes[i]->getModelMatrix()[0][0]);
			debugCubes[i]->draw(shader);
		}
	}
}
//Tests collision with other objects
void Player::collision(std::vector<Model*> &allModels)
{
	bool collides = true;
	int collisionChecks = 0;
	while (collides && collisionChecks < 5)
	{
		int index = -1;
		float minDistance = 1000;
		glm::vec2 player2dPos = glm::vec2(getPos().x, getPos().y + 0.5f);
		for (int i = 0; i < allModels.size(); i++)
		{
			float distance = glm::length(player2dPos - glm::vec2(allModels[i]->getModelMatrix()[3]));
			if (distance < minDistance)
			{
				minDistance = distance;
				index = i;
			}
		}

		if (index != -1)
		{
			std::vector<glm::vec2> playerPoints = getPoints();
			std::vector<glm::vec2> objectPoints;
			float radians = 0.0f;
			getPoints(objectPoints, allModels[index], radians);
			glm::vec2 mtv;
			if (collision::collision(playerPoints, objectPoints, mtv))
			{
				if (radians > 0.0f && radians < 0.79f)
				{
					if (mtv.y > 0)
					{
						modelMatrix[3].y -= 0.05f;
					}
					else
					{
						velocityY -= 0.5f;
					}
				}
				else
				{
					modelMatrix[3].x += mtv.x;
					if (mtv.y < 0)
					{
						velocityY = 0;
					}
				}
				modelMatrix[3].y += mtv.y;
				if (mtv.y > 0)
				{
					if (modelMatrix[3].y < 0) modelMatrix[3].y = 0;
					groundPos = modelMatrix[3].y;
				}
			}
			else
			{
				collides = false;
			}
		}
		collisionChecks++;
	}
}
void Player::getPoints(std::vector<glm::vec2> &objectPoints, Model *object, float &radians)
{
	//Get rotation and scale from modelMat
	glm::mat4 modelMat = object->getModelMatrix();
	glm::vec3 scale;
	glm::quat rotation;
	glm::decompose(modelMat, scale, rotation, glm::vec3(), glm::vec3(), glm::vec4());

	//Convert from quat to radians
	double t3 = +2.0 * (rotation.w * rotation.z + rotation.x * rotation.y);
	double t4 = +1.0 - 2.0f * ((rotation.y * rotation.y) + rotation.z * rotation.z);
	radians = (float)-std::atan2(t3, t4);

	//Get object points
	objectPoints = object->getPoints();
}

bool Player::TestRayOBBIntersection(
	glm::vec3 ray_origin,        // Ray origin, in world space
	glm::vec3 ray_direction,     // Ray direction (NOT target position!), in world space. Must be normalize()'d.
	glm::vec3 aabb_min,          // Minimum X,Y,Z coords of the mesh when not transformed at all.
	glm::vec3 aabb_max,          // Maximum X,Y,Z coords. Often aabb_min*-1 if your mesh is centered, but it's not always the case.
	glm::mat4 ModelMatrix,       // Transformation applied to the mesh (which will thus be also applied to its bounding box)
	float& intersection_distance // Output : distance between ray_origin and the intersection with the OBB
) {

	// Intersection method from Real-Time Rendering and Essential Mathematics for Games

	float tMin = 0.0f;
	float tMax = 100000.0f;

	glm::vec3 OBBposition_worldspace(ModelMatrix[3].x, ModelMatrix[3].y, ModelMatrix[3].z);

	glm::vec3 delta = OBBposition_worldspace - ray_origin;

	// Test intersection with the 2 planes perpendicular to the OBB's X axis
	{
		glm::vec3 xaxis(ModelMatrix[0].x, ModelMatrix[0].y, ModelMatrix[0].z);
		float e = glm::dot(xaxis, delta);
		float f = glm::dot(ray_direction, xaxis);

		if (fabs(f) > 0.001f) { // Standard case

			float t1 = (e + aabb_min.x) / f; // Intersection with the "left" plane
			float t2 = (e + aabb_max.x) / f; // Intersection with the "right" plane
											 // t1 and t2 now contain distances betwen ray origin and ray-plane intersections

											 // We want t1 to represent the nearest intersection, 
											 // so if it's not the case, invert t1 and t2
			if (t1>t2) {
				float w = t1;t1 = t2;t2 = w; // swap t1 and t2
			}

			// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
			if (t2 < tMax)
				tMax = t2;
			// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
			if (t1 > tMin)
				tMin = t1;

			// And here's the trick :
			// If "far" is closer than "near", then there is NO intersection.
			// See the images in the tutorials for the visual explanation.
			if (tMax < tMin)
				return false;

		}
		else { // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
			if (-e + aabb_min.x > 0.0f || -e + aabb_max.x < 0.0f)
				return false;
		}
	}


	// Test intersection with the 2 planes perpendicular to the OBB's Y axis
	// Exactly the same thing than above.
	{
		glm::vec3 yaxis(ModelMatrix[1].x, ModelMatrix[1].y, ModelMatrix[1].z);
		float e = glm::dot(yaxis, delta);
		float f = glm::dot(ray_direction, yaxis);

		if (fabs(f) > 0.001f) {

			float t1 = (e + aabb_min.y) / f;
			float t2 = (e + aabb_max.y) / f;

			if (t1>t2) { float w = t1;t1 = t2;t2 = w; }

			if (t2 < tMax)
				tMax = t2;
			if (t1 > tMin)
				tMin = t1;
			if (tMin > tMax)
				return false;

		}
		else {
			if (-e + aabb_min.y > 0.0f || -e + aabb_max.y < 0.0f)
				return false;
		}
	}


	// Test intersection with the 2 planes perpendicular to the OBB's Z axis
	// Exactly the same thing than above.
	{
		glm::vec3 zaxis(ModelMatrix[2].x, ModelMatrix[2].y, ModelMatrix[2].z);
		float e = glm::dot(zaxis, delta);
		float f = glm::dot(ray_direction, zaxis);

		if (fabs(f) > 0.001f) {

			float t1 = (e + aabb_min.z) / f;
			float t2 = (e + aabb_max.z) / f;

			if (t1>t2) { float w = t1;t1 = t2;t2 = w; }

			if (t2 < tMax)
				tMax = t2;
			if (t1 > tMin)
				tMin = t1;
			if (tMin > tMax)
				return false;

		}
		else {
			if (-e + aabb_min.z > 0.0f || -e + aabb_max.z < 0.0f)
				return false;
		}
	}

	intersection_distance = tMin;
	return true;

}