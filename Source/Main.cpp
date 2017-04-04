#include <GL/glew.h>
#include <GL/GL.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Shader.h"
#include "Camera.h"
//#include "Model.h"
//#include "FrustumCulling.h"

#pragma comment(lib, "opengl32.lib")

//Shader
Shader simpleShader;

//Camera
Camera playerCamera;
float verticalFOV = 45.0f;
int windowWidth = 1280.0f;
int windowHeight = 720.0f;
float nearDistance = 0.01f;
float farDistance = 1000;
glm::mat4 projectionMatrix = glm::perspective(verticalFOV, (float)windowWidth / (float)windowHeight, nearDistance, farDistance);
glm::mat4 viewMatrix;
//FrustumCulling frustumObject;
//VBO, VAO
GLuint VBO, VAO;

//Triangle
GLfloat vertices[] = {
	// Positions         // Colors
	0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // Bottom Right
	-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // Bottom Left
	0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f   // Top 
};
glm::mat4 triangleModelMat = {
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 3.0, 1.0 };

//Functions
void render();
void update(sf::Window &window);
void loadTriangle();

int main()
{
	// create the window
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 2;
	sf::Window window(sf::VideoMode(1280, 720), "OpenGL", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);

	// activate the window
	window.setActive(true);

	// load resources, initialize the OpenGL states, ...
	glewInit();

	simpleShader = Shader("Shaders/simpleVertex.glsl", "Shaders/simpleFragment.glsl");

	loadTriangle();

	// run the main loop
	bool running = true;
	while (running)
	{
		// handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				// end the program
				running = false;
			}
			else if (event.type == sf::Event::Resized)
			{
				// adjust the viewport when the window is resized
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

		// clear the buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		update(window);
		render();

		// end the current frame (internally swaps the front and back buffers)
		window.display();
	}

	// release resources...

	return 0;
}

void render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw the triangle
	simpleShader.use();
	glBindVertexArray(VAO);

	GLint viewID = glGetUniformLocation(simpleShader.program, "view");
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &viewMatrix[0][0]);
	GLint projectionID = glGetUniformLocation(simpleShader.program, "projection");
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(simpleShader.program, "model"), 1, GL_FALSE, &triangleModelMat[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}
void update(sf::Window &window)
{
	viewMatrix = playerCamera.Update(0.1f, window);
}
void loadTriangle()
{
	VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0); // Unbind VAO
}