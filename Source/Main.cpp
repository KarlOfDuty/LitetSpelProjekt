#include <GL/glew.h>
#include <GL/GL.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Shader.h"
#include "FreeCamera.h"
#include "Model.h"
#include "FrustumCulling.h"

#pragma comment(lib, "opengl32.lib")
const bool aboveView = false;
//gBuffer
Shader deferredGeometryPass;
Shader deferredLightingPass;
GLuint gBuffer;

//Textures
GLuint gPosition, gNormal, gAlbedoSpec, gAmbient;

//Camera
FreeCamera playerCamera;
float verticalFOV = 45.0f;
int windowWidth = 1280;
int windowHeight = 720;
float nearDistance = 0.01f;
float farDistance = 1000;
glm::mat4 projectionMatrix = glm::perspective(verticalFOV, (float)windowWidth / (float)windowHeight, nearDistance, farDistance);
glm::mat4 viewMatrix;
FrustumCulling frustumObject;
glm::vec4 mapSize = glm::vec4(-100.0f, -100.0f, 100.0f, 100.0f);

//Lights
const GLuint NR_LIGHTS = 10;
std::vector<glm::vec3> lightPositions;
std::vector<glm::vec3> lightColors;

//VBO VAO
GLuint VBO, VAO, EBO;
GLuint quadVAO = 0;
GLuint quadVBO;

//Models
std::vector<std::string> modelFilePaths = { "models/cube/cube.obj","models/sphere/sphere.obj" };
std::vector<Model*> modelLibrary;
std::vector<Model*> staticModels;
std::vector<Model*> visibleStaticModels;
std::vector<Model*> dynamicModels;

sf::Clock deltaClock;
sf::Time deltaTime;

//Functions
void render();
void update(sf::Window &window);
void createGBuffer();
void drawQuad();
void loadModels();
void setupModels();

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
	loadModels();
	setupModels();

	//Set up the frustum culling object and quadtree
	frustumObject.setFrustumShape(verticalFOV, (float)windowWidth / (float)windowHeight, nearDistance, farDistance);
	frustumObject.getRoot()->buildQuadTree(staticModels, 0, mapSize);
	frustumObject.getRoot()->cleanTree();

	//Main loop
	bool running = true;
	while (running)
	{
		//Handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				//End the program
				running = false;
			}
			else if (event.type == sf::Event::Resized)
			{
				//Adjust the viewport when the window is resized
				glViewport(0, 0, event.size.width, event.size.height);
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					running = false;
				}
			}
		}

		//Clear the buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		update(window);
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
	for (int i = 0; i < visibleStaticModels.size(); i++)
	{
		glUniformMatrix4fv(glGetUniformLocation(deferredGeometryPass.program, "model"), 1, GL_FALSE, &visibleStaticModels[i]->getModelMatrix()[0][0]);
		visibleStaticModels.at(i)->draw(deferredGeometryPass);
	}
	//All dynamic models are always drawn
	for (int i = 0; i < dynamicModels.size(); i++)
	{
		glUniformMatrix4fv(glGetUniformLocation(deferredGeometryPass.program, "model"), 1, GL_FALSE, &dynamicModels[i]->getModelMatrix()[0][0]);
		dynamicModels.at(i)->draw(deferredGeometryPass);
	}

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

	//Send all lights to the shader
	for (GLuint i = 0; i < lightPositions.size(); i++)
	{
		glUniform3fv(glGetUniformLocation(deferredLightingPass.program, ("lights[" + std::to_string(i) + "].position").c_str()), 1, &lightPositions[i][0]);
		glUniform3fv(glGetUniformLocation(deferredLightingPass.program, ("lights[" + std::to_string(i) + "].color").c_str()), 1, &lightColors[i][0]);
		// Linear and quadratic for calculation of the lights radius
		const GLfloat linear = 0.7f;
		const GLfloat quadratic = 1.8f;
		glUniform1f(glGetUniformLocation(deferredLightingPass.program, ("lights[" + std::to_string(i) + "].linear").c_str()), linear);
		glUniform1f(glGetUniformLocation(deferredLightingPass.program, ("lights[" + std::to_string(i) + "].quadratic").c_str()), quadratic);
	}
	glUniform3fv(glGetUniformLocation(deferredLightingPass.program, "viewPos"), 1, &playerCamera.getCameraPos()[0]);
	
	//Draw a fullscreen quad combining the information
	drawQuad();
}

//Update function
void update(sf::Window &window)
{
	deltaTime = deltaClock.restart();
	//Camera update, get new viewMatrix
	viewMatrix = playerCamera.Update(0.1f, window);
	if (aboveView)
	{
		playerCamera.Update(deltaTime.asSeconds(), window);
		viewMatrix = glm::lookAt(
			glm::vec3(0, 100, 0),
			glm::vec3(1, 1, 1),
			glm::vec3(0, 1, 0));
	}
	else
	{
		viewMatrix = playerCamera.Update(deltaTime.asSeconds(), window);
	}
	playerCamera.frustumCulling(frustumObject,visibleStaticModels);
	//TEMPORARY CAMERA CONTROLS, DISABLE WITH ALT
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
	{
		window.setMouseCursorVisible(true);
	}
	else
	{
		window.setMouseCursorVisible(false);
	}
}

//Create the buffer
void createGBuffer()
{
	//Load the shaders
	deferredGeometryPass = Shader("Shaders/gBufferGeometryVertex.glsl", "Shaders/gBufferGeometryFragment.glsl");
	deferredLightingPass = Shader("Shaders/gBufferLightingVertex.glsl", "Shaders/gBufferLightingFragment.glsl");

	// Set samplers
	deferredLightingPass.use();
	glUniform1i(glGetUniformLocation(deferredLightingPass.program, "gPosition"), 0);
	glUniform1i(glGetUniformLocation(deferredLightingPass.program, "gNormal"), 1);
	glUniform1i(glGetUniformLocation(deferredLightingPass.program, "gAlbedoSpec"), 2);
	glUniform1i(glGetUniformLocation(deferredLightingPass.program, "gAmbient"), 3);

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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windowWidth, windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gAmbient, 0);

	// - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
	GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
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

	//Some lights with random values
	std::srand(13);
	for (int i = 0; i < NR_LIGHTS; i++)
	{
		GLfloat xPos = 2.0f;
		GLfloat yPos = 2.0f;
		GLfloat zPos = 4.0f;
		lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
		GLfloat rColor = 0.6f;
		GLfloat gColor = 0.9f;
		GLfloat bColor = 0.9f;
		lightColors.push_back(glm::vec3(rColor, gColor, bColor));
	}
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

void loadModels()
{
	//Loads all models
	for (int i = 0; i < modelFilePaths.size(); i++)
	{
		modelLibrary.push_back(new Model(modelFilePaths[i]));
	}
}

void setupModels()
{
	std::srand(time(0));
	//Loads 100 spheres randomly
	for (int i = 0; i < 100; i++)
	{
		staticModels.push_back(new Model(modelLibrary.at(1), {
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			(rand() % 100) - 50, (rand() % 10) - 5, (rand() % 100) - 50, 1.0 }));
	}
	visibleStaticModels = staticModels;
}