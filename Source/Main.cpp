#include <GL/glew.h>
#include <GL/GL.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Audio.hpp>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Shader.h"
#include "FreeCamera.h"
#include "Camera.h"
#include "Player.h"
#include "EnemyManager.h"
#include "Model.h"
#include "FrustumCulling.h"
#include "EventHandler.h"
#include "LevelManager.h"
#include "SoundSystem.h"
#include "DirectionalLight.h"

#pragma comment(lib, "opengl32.lib")

LevelManager levelManager;
//Player
Player *player;
EventHandler eventHandler;
sf::Clock deltaClock;
float dt;
bool firstFrame = true;
//Enemies
EnemyManager *enemyManager;
int jumpPress;
bool keyReleased;

bool endLevel = false;
const bool aboveView = false;

//gBuffer
Shader deferredGeometryPass;
Shader deferredLightingPass;
GLuint gBuffer;
Shader shadowShader;

//Shadows
std::vector<GLuint> depthMap;
std::vector<GLuint> depthMapFBO;
const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

//Textures
GLuint gPosition, gNormal, gAlbedoSpec, gAmbient;

//Camera
Camera playerCamera;
float verticalFOV = 45.0f;
int windowWidth = 1280;
int windowHeight = 720;
float nearDistance = 0.01f;
float farDistance = 10000;
glm::mat4 projectionMatrix = glm::perspective(verticalFOV, (float)windowWidth / (float)windowHeight, nearDistance, farDistance);
glm::mat4 viewMatrix;

SoundSystem *soundSystem;

//Lights
std::vector<PointLight*> pointLights;
std::vector<DirectionalLight*> directionalLights;
//VBO VAO
GLuint VBO, VAO, EBO;
GLuint quadVAO = 0;
GLuint quadVBO;

std::vector<Model*> modelsToBeDrawn;

//Functions
void render();
void update(sf::Window &window);
void createGBuffer();
void drawQuad();
void loadLevel();
void unloadLevel();

//Main function
int main()
{
	//Create the window
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 2;
	sf::Window window(sf::VideoMode(windowWidth, windowHeight), "OpenGL", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	//Activate the window
	window.setActive(true);

	//Load resources, initialize the OpenGL states, ...
	glewInit();
	glEnable(GL_DEPTH_TEST);


	//Characters
	player = new Player();
	enemyManager = new EnemyManager();

	//Event handler
	eventHandler = EventHandler();

	//Level manager
	levelManager = LevelManager();
	playerCamera = Camera();
	playerCamera.setupFrustum(verticalFOV, windowWidth, windowHeight, nearDistance, farDistance);
	//Load level
	loadLevel();
	//Create the gbuffer textures and lights
	createGBuffer();
	player->setStaticModels(levelManager.currentLevel->getStaticModels());

	eventHandler = EventHandler();

	//Sound system
	soundSystem = new SoundSystem();
	soundSystem->loadSound("audio/sharkman/bowRelease.flac","bowRelease");
	soundSystem->playMusic("audio/music/never.flac");

	//Main loop
	bool running = true;
	while (running)
	{
		running = eventHandler.handleEvents(window, player, soundSystem);
		//Clear the buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (!firstFrame)
		{
			update(window);
		}
		else
		{
			deltaClock.restart();
			firstFrame = false;
		}
		render();

		//End the current frame (internally swaps the front and back buffers)
		window.display();
	}
	//Release resources...
	return 0;
}

//Render function for all drawings
void render()
{
	//Render depth of scene to texture (from light's perspective)
	//Get light projection/view matrix.
	glm::mat4 lightProjection;
	glm::mat4 lightView;
	std::vector<glm::mat4> lightSpaceMatrix;
	GLfloat nearPlane = 0.01f;
	GLfloat farPlane = 50.0f;
	lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, nearPlane, farPlane);
	for (int i = 0; i < pointLights.size(); i++)
	{
		lightSpaceMatrix.push_back(glm::mat4());
		lightView = glm::lookAt(pointLights[i]->pos, glm::vec3(2,0,0), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix[i] = lightProjection * lightView;
		//Render scene from light's point of view
		shadowShader.use();
		glUniformMatrix4fv(glGetUniformLocation(shadowShader.program, ("lightSpaceMatrix[" + std::to_string(i) + "]").c_str()), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix[i]));
		glUniform1i(glGetUniformLocation(shadowShader.program, "index"), i);
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[i]);
		glClear(GL_DEPTH_BUFFER_BIT);
		for (int j = 0; j < modelsToBeDrawn.size(); j++)
		{
			glUniformMatrix4fv(glGetUniformLocation(shadowShader.program, "model"), 1, GL_FALSE, &modelsToBeDrawn[j]->getModelMatrix()[0][0]);
			modelsToBeDrawn[j]->draw(shadowShader);
		}
		player->draw(shadowShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, windowWidth, windowHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// Geometry pass
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	deferredGeometryPass.use();

	//View and Projection Matrix
	GLint viewID = glGetUniformLocation(deferredGeometryPass.program, "view");
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &viewMatrix[0][0]);
	GLint projectionID = glGetUniformLocation(deferredGeometryPass.program, "projection");
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projectionMatrix[0][0]);

	//Draw functions

	//Only potentially visible static models are drawn
	for (int i = 0; i < modelsToBeDrawn.size(); i++)
	{
		glUniformMatrix4fv(glGetUniformLocation(deferredGeometryPass.program, "model"), 1, GL_FALSE, &modelsToBeDrawn[i]->getModelMatrix()[0][0]);
		modelsToBeDrawn.at(i)->draw(deferredGeometryPass);
	}
	if (player->playerIsDead() != true)
	{
		player->draw(deferredGeometryPass);
	}
	enemyManager->draw(deferredGeometryPass);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Lighting pass
	deferredLightingPass.use();

	//Bind all the textures
	int textureID = 0;
	glActiveTexture(GL_TEXTURE0 + textureID++);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glActiveTexture(GL_TEXTURE0 + textureID++);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE0 + textureID++);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glActiveTexture(GL_TEXTURE0 + textureID++);
	glBindTexture(GL_TEXTURE_2D, gAmbient);
	for (int i = 0; i < pointLights.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + textureID++);
		glBindTexture(GL_TEXTURE_2D, depthMap[i]);
	}
	//Send all lights to the shader
	for (GLuint i = 0; i < pointLights.size(); i++)
	{
		glUniform3fv(glGetUniformLocation(deferredLightingPass.program, ("pointLights[" + std::to_string(i) + "].position").c_str()), 1, &pointLights[i]->pos[0]);
		glUniform3fv(glGetUniformLocation(deferredLightingPass.program, ("pointLights[" + std::to_string(i) + "].color").c_str()), 1, &pointLights[i]->colour[0]);
		// Linear and quadratic for calculation of the lights radius
		glUniform1f(glGetUniformLocation(deferredLightingPass.program, ("pointLights[" + std::to_string(i) + "].linear").c_str()), pointLights[i]->linear);
		glUniform1f(glGetUniformLocation(deferredLightingPass.program, ("pointLights[" + std::to_string(i) + "].quadratic").c_str()), pointLights[i]->quadratic);
		glUniformMatrix4fv(glGetUniformLocation(deferredLightingPass.program, ("lightSpaceMatrix[" + std::to_string(i) + "]").c_str()), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix[i]));
	}
	glUniform1i(glGetUniformLocation(deferredLightingPass.program, "numberOfPointLights"), pointLights.size());
	glUniform1i(glGetUniformLocation(deferredLightingPass.program, "numberOfDirLights"), directionalLights.size());
	glUniform3fv(glGetUniformLocation(deferredLightingPass.program, "viewPos"), 1, &playerCamera.getCameraPos()[0]);
	//Draw a fullscreen quad combining the information
	drawQuad();
}

//Update function
void update(sf::Window &window)
{
	dt = deltaClock.restart().asSeconds();
	//Update player if not dead
	if (!player->playerIsDead())
	{
		player->update(window, dt, levelManager.currentLevel->getStaticModels() , enemyManager->getAllEnemies());
	}

	enemyManager->update(dt, player->getPos(), player->getDamage(), modelsToBeDrawn, player->getPoints());

	//Camera update, get new viewMatrix
	if (aboveView)
	{
		playerCamera.update(player->getPos());
		viewMatrix = glm::lookAt(
			glm::vec3(0, 100, 0),
			glm::vec3(1, 1, 1),
			glm::vec3(0, 1, 0));
	}
	else
	{
		viewMatrix = playerCamera.update(player->getPos());
	}

	if (endLevel)
	{
		unloadLevel();
		loadLevel();
		endLevel = false;
	}
	levelManager.currentLevel->updateTriggers(dt);
	playerCamera.frustumCulling(modelsToBeDrawn);
}

//Create the buffer
void createGBuffer()
{
	//Load the shaders
	deferredGeometryPass = Shader("Shaders/gBufferGeometryVertex.glsl", "Shaders/gBufferGeometryFragment.glsl");
	deferredLightingPass = Shader("Shaders/gBufferLightingVertex.glsl", "Shaders/gBufferLightingFragment.glsl");
	shadowShader = Shader("shadowVertex.glsl", "shadowFragment.glsl");

	for (int i = 0; i < pointLights.size(); i++)
	{
		//Shadow buffer
		depthMap.push_back(0);
		depthMapFBO.push_back(0);
		glGenFramebuffers(1, &depthMapFBO[0]);

		glGenTextures(1, &depthMap[0]);
		glBindTexture(GL_TEXTURE_2D, depthMap[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[0]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap[0], 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


	//Set samplers
	deferredLightingPass.use();
	int index = 0;
	glUniform1i(glGetUniformLocation(deferredLightingPass.program, "gPosition"), index++);
	glUniform1i(glGetUniformLocation(deferredLightingPass.program, "gNormal"), index++);
	glUniform1i(glGetUniformLocation(deferredLightingPass.program, "gAlbedoSpec"), index++);
	glUniform1i(glGetUniformLocation(deferredLightingPass.program, "gAmbient"), index++);
	for (int i = 0; i < pointLights.size(); i++)
	{
		glUniform1i(glGetUniformLocation(deferredLightingPass.program, ("depthMap[" + std::to_string(i) + "]").c_str()), index++);
	}

	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	//Position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	//Normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	//Color + Specular color buffer
	glGenTextures(1, &gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windowWidth, windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

	//Ambient colour buffer
	glGenTextures(1, &gAmbient);
	glBindTexture(GL_TEXTURE_2D, gAmbient);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gAmbient, 0);

	// - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
	GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, attachments);

	GLuint rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowWidth, windowHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// - Finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

//Quad used for lighting pass fullscreen quad
void drawQuad()
{
	if (quadVAO == 0)
	{
		GLfloat quadVertices[] = {
			// Positions        // Texture Coords
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void loadLevel()
{
	levelManager.currentLevel->loadModels();
	levelManager.currentLevel->setupModels();
	levelManager.currentLevel->setupTriggers(player);
	modelsToBeDrawn = levelManager.currentLevel->getStaticModels();

	enemyManager->createSlime(glm::vec3(18.0f, 7.0f, 0.0f));
	enemyManager->createToad(glm::vec3(-16.0f, 7.0f, 0.0f));
	enemyManager->createGiantBat(glm::vec3(25.0f, 12.0f, 0.0f));
	enemyManager->createBatSwarm(glm::vec3(-16.2f, 5.8f, 0.0f));
	enemyManager->createBatSwarm(glm::vec3(-15.0f, 5.3f, 0.0f));
	enemyManager->createBatSwarm(glm::vec3(-14.0f, 5.6f, 0.0f));
	enemyManager->createCrab(glm::vec3(-30.0f, 7.0f, 0.0f));
	enemyManager->createFirefly(glm::vec3(-15.0f, 6.0f, 0.0f));
	enemyManager->createSkeleton(glm::vec3(30.0f, 7.0f, 0.0f), false);
	playerCamera.setupQuadTree(levelManager.currentLevel->getStaticModels());
	//Some lights with random values
	std::srand((int)time(0));
	pointLights.push_back(new PointLight(
		glm::vec3(-3.0f, 10.0f, 0),
		glm::vec3(0.6f, 0.9f, 0.9f),
		0.0000f, 0.00f));
	directionalLights.push_back(new DirectionalLight(
		glm::vec3(-3.0f, 10.0f, 0),
		glm::vec3(0.6f, 0.9f, 0.9f)));
	player->setPos(levelManager.currentLevel->getPlayerPos());
}
void unloadLevel()
{
	levelManager.currentLevel->unloadModels();
	levelManager.currentLevel->deleteTriggers();
	modelsToBeDrawn.clear();
	playerCamera.destroyQuadTree();
	player->clearProjectiles();
	enemyManager->removeAll();
	for (int i = 0; i < pointLights.size(); i++)
	{
		delete pointLights[i];
	}
	pointLights.clear();
}