#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL\glew.h>
#include <GL\GL.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <SOIL.h>
#include "Shader.h"
#include "GameObject.h"
//A material specifying how shading, coloring and texturing works
struct Material
{
	//Material name
	std::string name;
	//Colours
	glm::vec3 ambientColour;
	glm::vec3 diffuseColour;
	glm::vec3 specularColour;
	//Textures
	GLuint ambientTexture;
	GLuint diffuseTexture;
	GLuint specularTexture;
	GLuint normalMapTexture;
	//File names for textures
	std::string textureMapAmbientFile;
	std::string textureMapDiffuseFile;
	std::string textureMapSpecularFile;
	std::string normalMapFile;
	//Searching Functions
	static int findMaterial(std::string name, std::vector<Material> materials);
};
//A single vertex (corner) in a face.
struct Vertex
{
	glm::vec3 pos;
	glm::vec2 texPos;
	glm::vec3 normal;
	int useNormalMap;
};
struct Mesh
{
	std::vector<Vertex> vertices;
	Material material;
};
//Turns on console feedback for reading of model files
static bool modelDebug = false;
//Turns on console feedback for reading of material files
static bool matDebug = true;
class Model : public GameObject
{
private:
	glm::mat4 modelMatrix;
	glm::mat4 rotationMatrix;
	std::vector<Mesh*> meshes;
	std::vector<glm::vec2> allPoints;
	glm::vec3 minBounding;
	glm::vec3 maxBounding;
	float boundingSphereRadius;
	void setupModel();
	void loadTextures(int meshNr);
public:
	//Parent inherited functions
	std::vector<glm::vec2> getPoints();
	glm::vec3 getPos() const;
	std::string type() const;
	//Own functions
	GLuint VAO; //Vertex Array Object
	GLuint VBO; //Vertex Buffer Object
	Material getMaterial(int index);
	glm::mat4 getModelMatrix() const;
	glm::mat4 getRotationMatrix() const;
	void getMinMaxBouding(glm::vec3 &min, glm::vec3 &max);
	void setModelMatrix(glm::mat4 modelMat);
	void setRotationMatrix(glm::mat4 rotationMat);
	void rotate();
	void read(std::string filename);
	void draw(Shader shader);
	void setBoundingSphereRadius();
	float getBoundingSphereRadius() const;
	//Constructors
	Model(std::string filename);
	Model(std::string filename, glm::mat4 modelMat);
	Model(std::string filename, glm::mat4 modelMat, glm::mat4 rotation);
	//Copy constructors
	Model(Model &otherModel);
	Model(Model *otherModel);
	Model(Model &otherModel, glm::mat4 modelMat);
	Model(Model *otherModel, glm::mat4 modelMat);
	Model();
	//Destructor
	~Model();
	void deleteMeshes();
};
#endif