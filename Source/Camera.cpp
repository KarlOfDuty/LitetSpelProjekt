#include "Camera.h"

Camera::Camera()
{	
	this->frustumObject = FrustumCulling();
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
	this->cameraPos.y = playerPos.y + 50;
	this->cameraPos.z = playerPos.z + 300;

	this->cameraFront = playerPos - cameraPos;
	this->cameraFront = glm::normalize(cameraFront);

	//The camera also points to a point slightly above the player
	return glm::lookAt(cameraPos, playerPos + glm::vec3(0, 0, 0), cameraUp);
}
//fix functio to rotate menu
glm::mat4 Camera::update(glm::vec3 playerPos, glm::vec3 menuOfset)
{
	//The camera is always in front of the player and slightly above.
	this->cameraPos.x = playerPos.x;
	this->cameraPos.y = playerPos.y + 50;
	this->cameraPos.z = playerPos.z + 300;

	this->cameraFront = playerPos - cameraPos;
	this->cameraFront = glm::normalize(cameraFront);

	//The camera also points to a point slightly above the player
	return glm::lookAt(cameraPos, playerPos + menuOfset, cameraUp);
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
void Camera::setupFrustum(float verticalFOV, float windowWidth, float windowHeight, float nearD, float farD)
{
	//Sets up the frustum using the same variables as the projection matrix
	frustumObject.setFrustumShape(verticalFOV, (float)windowWidth / (float)windowHeight, nearD, farD);
}
void Camera::setupQuadTree(std::vector<Model*> &staticModels)
{
	//Recursively sets the quadtree up starting from the root
	frustumObject.getRoot()->buildQuadTree(staticModels, 0, mapSize);
	//frustumObject.getRoot()->cleanTree();
}

void Camera::destroyQuadTree()
{
	frustumObject.destroyQuadTree();
}
