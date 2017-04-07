#include "Camera.h"

Camera::Camera()
{
	//this->cameraHasMoved = true;
	//this->firstMouse = true;
	//this->oldMouseX = resolutionWidth / 2;
	//this->oldMouseY = resolutionHeight / 2;
	//this->mouseSensitivity = 0.05f;
	//this->cameraSpeed = 0.05f;
	this->cameraPos = glm::vec3(0.0f, 0.0f, 2.0f);
	this->cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->cameraYaw = -90.0f;
	this->cameraPitch = 0.0f;
	this->resolutionWidth = sf::VideoMode::getDesktopMode().width;
	this->resolutionHeight = sf::VideoMode::getDesktopMode().height;
}

Camera::~Camera()
{

}
//Update function should only be run if the player has moved
glm::mat4 Camera::update(glm::vec3 playerPos)
{
	//The camera is always in front of the player and slightly above.
	cameraPos.x = playerPos.x;
	cameraPos.y = playerPos.y + 5;
	cameraPos.z = playerPos.z + 5;
	//The camera also points to a point slightly above the player
	return glm::lookAt(cameraPos, playerPos + glm::vec3(0, 2, 0), cameraUp);
}
glm::vec3 Camera::getCameraPos()
{
	return cameraPos;
}
//Performs frustum culling from this camera's point of view, should only be run if camera has changed
void Camera::frustumCulling(FrustumCulling &fcObject, std::vector<Model*> &visibleModels)
{
	//TODO: Fix cameraFront
	//If the camera has moved, update the visible models
	fcObject.setFrustumPlanes(cameraPos, cameraFront, cameraUp);
	visibleModels = fcObject.getRoot()->getModelsToDraw(fcObject);
	//Remove duplicate pointers
	std::sort(visibleModels.begin(), visibleModels.end());
	visibleModels.erase(std::unique(visibleModels.begin(), visibleModels.end()), visibleModels.end());
}