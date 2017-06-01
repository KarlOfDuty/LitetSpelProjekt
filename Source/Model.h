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
#include <glm/gtx/transform.hpp> 
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
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
struct Joint
{
	int animationIndex;
	int nrOfClusters;
	std::string jointName;
	glm::mat4 globalBindPosMat;
	std::vector<glm::mat4> transformMat;
	int nrOfKeys;
};
struct Weights
{
	int nrOfIndices;
	std::vector<glm::ivec4> controllers;
	std::vector<glm::vec4> weightsInfluence;
};
struct Vertex
{
	glm::vec3 pos;
	glm::vec2 texPos;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 biTangent;
	int useNormalMap;
};
struct Mesh
{
	std::string name;
	std::vector<Vertex> vertices;
	Material material;
};
//Turns on console feedback for reading of model files
static bool modelDebug = false;
//Turns on console feedback for reading of material files
static bool matDebug = false;
//Shows collision boxes
static bool showColliders = false;
class Model : public GameObject
{
private:
	glm::mat4 currentJointTrans[100];
	int nrOfKeyframes = 0;
	int currentFrame = 0;
	bool hasAnimations;
	glm::mat4 modelMatrix;
	glm::mat4 rotationMatrix;
	std::vector<Mesh*> meshes;
	std::vector<Joint*> skeleton;
	Weights weightInfo;
	std::vector<glm::vec2> allPoints;
	glm::vec3 minBounding;
	glm::vec3 maxBounding;
	float boundingSphereRadius;
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
	void getScaledMinMaxBouding(glm::vec3 & min, glm::vec3 & max);
	void getScaledMinMaxBouding(glm::vec3 & min, glm::vec3 & max, glm::mat4 modelMat);
	void setModelMatrix(glm::mat4 modelMat);
	void setRotationMatrix(glm::mat4 rotationMat);
	void setRotationMatrix(glm::vec3 rotation);
	void setPos(glm::vec3 pos);
	void setScale(glm::vec3& scale);
	void addMesh(Mesh* mesh);
	void rotate();
	void readOBJ(std::string filename);
	bool readModel(const char* filePath);
	void loadSkeleton(const char* filePath);
	void loadWeight(const char* filePath);
	void setupModel();
	void loadTextures(int meshNr);
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