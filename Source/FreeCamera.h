#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <SFML\Window.hpp>
#include "FrustumCulling.h"
extern const bool aboveView;
class FreeCamera
{
private:
	bool firstMouse;
	bool cameraHasMoved;
	int oldMouseX;
	int oldMouseY;
	float mouseSensitivity;
	float cameraSpeed;
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	float cameraYaw;
	float cameraPitch;
	int resolutionWidth;
	int resolutionHeight;
public:
	FreeCamera();
	~FreeCamera();
	void frustumCulling(FrustumCulling &fcObject, std::vector<Model*> &models);
	glm::mat4 Update(float deltaTime, sf::Window &window);
	glm::vec3 getCameraPos();
	void setupQuadTree(std::vector<Model*> &staticModels);
};

