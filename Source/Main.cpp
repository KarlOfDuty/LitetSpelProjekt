#include <GL/glew.h>
#include <GL/GL.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Shader.h"
#include "FreeCamera.h"
#include "Camera.h"
#include "Player.h"
#include "Enemy.h"
#include "Model.h"
#include "FrustumCulling.h"
#include "EventHandler.h"
#include "LevelManager.h"
#include "Light.h"
#pragma comment(lib, "opengl32.lib")

LevelManager levelManager;

//Player
Player *player;
EventHandler eventHandler;
sf::Clock deltaClock;
sf::Clock timer;
float dt;
//Enemies
Enemy *enemy;
int jumpPress;
bool keyReleased;
const bool aboveView = false;

//gBuffer
Shader deferredGeometryPass;
Shader deferredLightingPass;
GLuint gBuffer;
Shader simpleShadowShader;

//Shadows
GLuint depthMap;
GLuint depthMapFBO;
const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

// Light source
glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);

//Textures
GLuint gPosition, gNormal, gAlbedoSpec, gAmbient;

//Camera
FreeCamera freeCamera;
Camera playerCamera;
float verticalFOV = 45.0f;
int windowWidth = 1280;
int windowHeight = 720;
float nearDistance = 0.01f;
float farDistance = 10000;
glm::mat4 projectionMatrix = glm::perspective(verticalFOV, (float)windowWidth / (float)windowHeight, nearDistance, farDistance);
glm::mat4 viewMatrix;

//Lights
const GLuint NR_LIGHTS = 10;

//VBO VAO
GLuint VBO, VAO, EBO;
GLuint quadVAO = 0;
GLuint quadVBO;

std::vector<Model*> modelsToBeDrawn;
std::vector<Light*> lights;
bool unloaded = false;
bool reloaded = false;
//Functions
void render();
void update(sf::Window &window);
void createGBuffer();
void drawQuad();
void loadLevel();
void unloadLevel();
void reloadLevel();

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
	//Create the gbuffer textures and lights
	createGBuffer();

	//Models
	loadLevel();

	//Player
	player = new Player();
	enemy = new Enemy();
	enemy->createSlime(glm::vec3(10.0f, 0.0f, 0.0f));
	// run the main loop
	eventHandler = EventHandler();

	timer.restart();

	//Main loop
	bool running = true;
	while (running)
	{
		running = eventHandler.handleEvents(window, player);
		//Clear the buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		update(window);
		unloadLevel();
		reloadLevel();
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
	// 1. Render depth of scene to texture (from ligth's perspective)
	// - Get light projection/view matrix.
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	GLfloat near_plane = 0.01f, far_plane = 7.5f;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	// - now render scene from light's point of view
	simpleShadowShader.use();
	glUniformMatrix4fv(glGetUniformLocation(simpleShadowShader.program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	for (int i = 0; i < modelsToBeDrawn.size(); i++)
	{
		glUniformMatrix4fv(glGetUniformLocation(simpleShadowShader.program, "model"), 1, GL_FALSE, &modelsToBeDrawn[i]->getModelMatrix()[0][0]);
		modelsToBeDrawn.at(i)->draw(simpleShadowShader);
	}
	player->draw(simpleShadowShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, windowWidth, windowHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
	if (player->playerDead() != true)
	{
		player->draw(deferredGeometryPass);
	}
	enemy->draw(deferredGeometryPass);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Lighting pass
	deferredLightingPass.use();
	
	//Bind all the textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gAmbient);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	//Send all lights to the shader
	for (GLuint i = 0; i < lights.size(); i++)
	{
		glUniform3fv(glGetUniformLocation(deferredLightingPass.program, ("lights[" + std::to_string(i) + "].position").c_str()), 1, &lights[i]->pos[0]);
		glUniform3fv(glGetUniformLocation(deferredLightingPass.program, ("lights[" + std::to_string(i) + "].color").c_str()), 1, &lights[i]->colour[0]);
		// Linear and quadratic for calculation of the lights radius
		glUniform1f(glGetUniformLocation(deferredLightingPass.program, ("lights[" + std::to_string(i) + "].linear").c_str()), lights[i]->linear);
		glUniform1f(glGetUniformLocation(deferredLightingPass.program, ("lights[" + std::to_string(i) + "].quadratic").c_str()), lights[i]->quadratic);
	}
	glUniformMatrix4fv(glGetUniformLocation(deferredLightingPass.program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
	glUniform3fv(glGetUniformLocation(deferredLightingPass.program, "viewPos"), 1, &playerCamera.getCameraPos()[0]);
	
	//Draw a fullscreen quad combining the information
	drawQuad();
}

//Update function
void update(sf::Window &window)
{	
	dt = deltaClock.restart().asSeconds();
	//Camera update, get new viewMatrix
	if (aboveView)
	{
		playerCamera.update(player->getPlayerPos());
		viewMatrix = glm::lookAt(
			glm::vec3(0, 100, 0),
			glm::vec3(1, 1, 1),
			glm::vec3(0, 1, 0));
	}
	else
	{
		viewMatrix = playerCamera.update(player->getPlayerPos());
		//viewMatrix = freeCamera.Update(dt,window);
	}
	if (player->playerDead() != true)
	{
		player->update(dt, modelsToBeDrawn ,enemy->getEnemyPos(), enemy->getDamage());
	}
		enemy->update(dt, player->getPlayerPos());
	//playerCamera.frustumCulling(modelsToBeDrawn);
}

//Create the buffer
void createGBuffer()
{
	//Load the shaders
	deferredGeometryPass = Shader("Shaders/gBufferGeometryVertex.glsl", "Shaders/gBufferGeometryFragment.glsl");
	deferredLightingPass = Shader("Shaders/gBufferLightingVertex.glsl", "Shaders/gBufferLightingFragment.glsl");
	simpleShadowShader = Shader("simpleVertex.glsl", "simpleFragment.glsl");


	//Shadow buffer
	glGenFramebuffers(1, &depthMapFBO);

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	// Set samplers
	deferredLightingPass.use();
	glUniform1i(glGetUniformLocation(deferredLightingPass.program, "gPosition"), 0);
	glUniform1i(glGetUniformLocation(deferredLightingPass.program, "gNormal"), 1);
	glUniform1i(glGetUniformLocation(deferredLightingPass.program, "gAlbedoSpec"), 2);
	glUniform1i(glGetUniformLocation(deferredLightingPass.program, "gAmbient"), 3);
	glUniform1i(glGetUniformLocation(deferredLightingPass.program, "depthMap"), 4);

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
	modelsToBeDrawn = levelManager.currentLevel->getStaticModels();
	//std::cout << levelManager.currentLevel->getStaticModels().size() << std::endl;
	//playerCamera.setupQuadTree(levelManager.currentLevel->getStaticModels());

	//Some lights with random values
	std::srand(time(0));
	for (int i = 0; i < NR_LIGHTS; i++)
	{
		lights.push_back(new Light(
			rand()%50-25,2.0f,4.0f,
			0.6f,0.9f,0.9f,
			0.0001f,0.02f));
	}
}
void unloadLevel()
{
	//if (timer.getElapsedTime().asSeconds() > 3)
	//{
		levelManager.currentLevel->unloadModels();
		std::cout << "Done" << std::endl;
		modelsToBeDrawn.clear();
		//playerCamera.destroyQuadTree();
		//for (int i = 0; i < lights.size(); i++)
		//{
		//	delete lights[i];
		//}
		//lights.clear();
		//unloaded = true;
	//}
}
void reloadLevel()
{
	//if (timer.getElapsedTime().asSeconds() > 6)
	//{
		levelManager.currentLevel->loadModels();
		levelManager.currentLevel->setupModels();
		modelsToBeDrawn = levelManager.currentLevel->getStaticModels();
		//std::cout << levelManager.currentLevel->getStaticModels().size() << std::endl;
		//playerCamera.setupQuadTree(levelManager.currentLevel->getStaticModels());

		//Some lights with random values
		//std::srand(time(0));
		//for (int i = 0; i < NR_LIGHTS; i++)
		//{
		//	lights.push_back(new Light(
		//		rand() % 50 - 25, 2.0f, 4.0f,
		//		0.6f, 0.9f, 0.9f,
		//		0.0001f, 0.02f));
		//}
		//reloaded = true;
	//}
}