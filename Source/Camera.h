#ifndef CAMERA_H
#define CAMERA_H
#include <glm\glm.hpp>
#include <SFML/Window.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "FrustumCulling.h"
#include "Model.h"
class Camera
{
public:
	Camera();
	~Camera();
	void frustumCulling(FrustumCulling &fcObject, std::vector<Model*> &visibleModels);
	glm::mat4 update(glm::vec3 playerPos);
	glm::vec3 getCameraPos();
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
};
#endif