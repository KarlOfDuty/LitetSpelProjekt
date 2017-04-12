#include "Camera.h"

Camera::Camera()
{
	this->cameraPos = glm::vec3(0.0f, 0.0f, 2.0f);
	this->cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->mapSize = glm::vec4(-100.0f, -100.0f, 100.0f, 100.0f);
}

Camera::~Camera()
{

}
//Update function should only be run if the player has moved
glm::mat4 Camera::update(glm::vec3 playerPos)
{
	//The camera is always in front of the player and slightly above.
	this->cameraPos.x = playerPos.x;
	this->cameraPos.y = playerPos.y + 5;
	this->cameraPos.z = playerPos.z + 10;

	this->cameraFront = (playerPos + glm::vec3(0,2,0)) - cameraPos;
	this->cameraFront = glm::normalize(cameraFront);

	//The camera also points to a point slightly above the player
	return glm::lookAt(cameraPos, playerPos + glm::vec3(0, 2, 0), cameraUp);
}
glm::vec3 Camera::getCameraPos()
{
	return cameraPos;
}
//Performs frustum culling from this camera's point of view, should only be run if camera has changed
void Camera::frustumCulling(std::vector<Model*> &visibleModels)
{
	//If the camera has moved, update the visible models
	frustumObject.setFrustumPlanes(cameraPos, cameraFront, cameraUp);
	visibleModels = frustumObject.getRoot()->getModelsToDraw(frustumObject);
	//Remove duplicate pointers
	std::sort(visibleModels.begin(), visibleModels.end());
	visibleModels.erase(std::unique(visibleModels.begin(), visibleModels.end()), visibleModels.end());
}

void Camera::setupQuadTreeAndFrustum(float verticalFOV, float windowWidth, float windowHeight, float nearD, float farD, std::vector<Model*> &staticModels)
{
	//Set up the frustum culling object and quadtree
	frustumObject = FrustumCulling();
	frustumObject.setFrustumShape(verticalFOV, (float)windowWidth / (float)windowHeight, nearD, farD);
	frustumObject.getRoot()->buildQuadTree(staticModels, 0, mapSize);
	frustumObject.getRoot()->cleanTree();
}