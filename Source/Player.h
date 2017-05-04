#ifndef PLAYER_H
#define PLAYER_H
#include "PlayerBird.h"
#include "PlayerShark.h"
#include "PlayerButterfly.h"
#include "Shader.h"
#include "Collision.h"
#include "Projectile.h"
#include "GameObject.h"
#include "Enemy.h"
#include <SFML\Window.hpp>
#include <glm\glm.hpp>
#include <vector>
#include <iostream>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Player : public GameObject
{
private:
	PlayerChar* playerCharacters[3];
	PlayerChar* player;
	void freeMemory();
	sf::Clock damageImmunity;
	sf::Clock tpCooldown;
	glm::mat4 modelMatrix;
	glm::mat4 rotationMatrix;
	float velocityX;
	float velocityY;
	bool goingLeft;
	bool goingRight;
	float angle;
	bool isOnGround;
	int jumps;
	float movementSpeed;
	float groundPos;
	enum { CONTROLLER0, CONTROLLER1, CONTROLLER2, CONTROLLER3 };
	std::vector<Model*> debugCubes;
	
	Model* arrow;
	std::vector<Projectile*> arrows;

public:
	//Parent inherited functions
	std::vector<glm::vec2> getPoints();
	glm::vec3 getPos() const;
	std::string type() const;
	//Own functions
	Player();
	~Player();
	PlayerChar* getCurrentCharacter();
	void swap(int charType);
	bool playerIsDead();
	int getDamage() const;
	void update(sf::Window &window, float dt, std::vector<Model*> &allModels, std::vector<Enemy*> allEnemies);
	void jump();
	void lightAttackPressed(sf::Window &window);
	void lightAttackReleased(sf::Window &window);
	void heavyAttackPressed(sf::Window &window);
	void heavyAttackReleased(sf::Window &window);
	void aiming(sf::Window &window, float dt);
	void setPos(glm::vec3 playerPos);
	void draw(Shader shader);
	void collision(std::vector<Model*> &allModels);
	void getPoints(std::vector<glm::vec2> &objectPoints, Model *object, float &radians);

	bool TestRayOBBIntersection(
		glm::vec3 ray_origin,        // Ray origin, in world space
		glm::vec3 ray_direction,     // Ray direction (NOT target position!), in world space. Must be normalize()'d.
		glm::vec3 aabb_min,          // Minimum X,Y,Z coords of the mesh when not transformed at all.
		glm::vec3 aabb_max,          // Maximum X,Y,Z coords. Often aabb_min*-1 if your mesh is centered, but it's not always the case.
		glm::mat4 ModelMatrix,       // Transformation applied to the mesh (which will thus be also applied to its bounding box)
		float& intersection_distance // Output : distance between ray_origin and the intersection with the OBB
	);
};
#endif
