#include "FreeCamera.h"
FreeCamera::FreeCamera()
{
	this->cameraHasMoved = true;
	this->firstMouse = true;
	this->oldMouseX = resolutionWidth / 2;
	this->oldMouseY = resolutionHeight / 2;
	this->mouseSensitivity = 0.05f;
	this->cameraSpeed = 0.05f;
	this->cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
	this->cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->cameraYaw = -90.0f;
	this->cameraPitch = 0.0f;
	this->resolutionWidth = sf::VideoMode::getDesktopMode().width;
	this->resolutionHeight = sf::VideoMode::getDesktopMode().height;
}
FreeCamera::~FreeCamera()
{

}
void FreeCamera::frustumCulling(FrustumCulling &fcObject, std::vector<Model*> &visibleModels)
{
	//If the camera has moved, update the visible models
	if (cameraHasMoved)
	{
		fcObject.setFrustumPlanes(cameraPos, cameraFront, cameraUp);
		visibleModels = fcObject.getRoot()->getModelsToDraw(fcObject);
		//Remove duplicate pointers
		std::sort(visibleModels.begin(), visibleModels.end());
		visibleModels.erase(std::unique(visibleModels.begin(), visibleModels.end()), visibleModels.end());
		cameraHasMoved = false;
	}
}
//Updates the camera
glm::mat4 FreeCamera::Update(float deltaTime, sf::Window &window)
{
	if (window.hasFocus() && !sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
	{
		//Used for movement
		glm::vec3 flattenedCameraFront = cameraFront;
		flattenedCameraFront.y = 0;
		flattenedCameraFront = glm::normalize(flattenedCameraFront);

		cameraSpeed = 0.5f * deltaTime;
		//Forward
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			cameraPos.x += (cameraSpeed * flattenedCameraFront).x;
			cameraPos.z += (cameraSpeed * flattenedCameraFront).z;
			cameraHasMoved = true;
		}
		//Backwards
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			cameraPos.x -= (cameraSpeed * flattenedCameraFront).x;
			cameraPos.z -= (cameraSpeed * flattenedCameraFront).z;
			cameraHasMoved = true;
		}
		//Left
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			cameraPos -= glm::normalize(glm::cross(flattenedCameraFront, cameraUp)) * cameraSpeed*5.0f;
			cameraHasMoved = true;
		}
		//Right
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			cameraPos += glm::normalize(glm::cross(flattenedCameraFront, cameraUp)) * cameraSpeed*5.0f;
			cameraHasMoved = true;
		}
		//Executed only once on startup
		if (firstMouse)
		{
			sf::Mouse::setPosition(sf::Vector2i(resolutionWidth / 2, resolutionHeight / 2));
			oldMouseX = sf::Mouse::getPosition().x;
			oldMouseY = sf::Mouse::getPosition().y;
			firstMouse = false;
		}
		//Checks if the mouse has been moved from the center
		if (sf::Mouse::getPosition().x != resolutionWidth / 2)
		{
			cameraYaw += (float)(sf::Mouse::getPosition().x - oldMouseX) * mouseSensitivity;
			cameraHasMoved = true;
		}
		if (sf::Mouse::getPosition().y != resolutionHeight / 2 && !aboveView)
		{
			cameraPitch += (float)(oldMouseY - sf::Mouse::getPosition().y) * mouseSensitivity;
			cameraHasMoved = true;
		}
		//Limits the pitch so the player cant move the camera further than straight up or down
		if (cameraPitch > 89.0f)
		{
			cameraPitch = 89.0f;
		}
		if (cameraPitch < -89.0f)
		{
			cameraPitch = -89.0f;
		}
		//Calculates new camera variables
		glm::vec3 front;
		front.x = cos(glm::radians(cameraPitch)) * cos(glm::radians(cameraYaw));
		front.y = sin(glm::radians(cameraPitch));
		front.z = cos(glm::radians(cameraPitch)) * sin(glm::radians(cameraYaw));
		cameraFront = glm::normalize(front);
		//Resets the mouse position
		sf::Mouse::setPosition(sf::Vector2i(resolutionWidth / 2, resolutionHeight / 2));
		oldMouseX = sf::Mouse::getPosition().x;
		oldMouseY = sf::Mouse::getPosition().y;
	}
	//Returns the view matrix
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}
glm::vec3 FreeCamera::getCameraPos()
{
	return this->cameraPos;
}