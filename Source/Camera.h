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
	void frustumCulling(std::vector<Model*> &visibleModels);
	void setupFrustum(float verticalFOV, float windowWidth, float windowHeight, float nearD, float farD);
	void setupQuadTree(std::vector<Model*> &staticModels);
	glm::mat4 update(glm::vec3 playerPos);
	glm::vec3 getCameraPos();
private:
	FrustumCulling frustumObject;
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec4 mapSize;
};
#endif