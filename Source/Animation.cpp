#include <glm\glm.hpp>
#include <glm/gtx/common.hpp>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <string>
#include <gl\glew.h>
#include <fstream>
#include <iostream>
using namespace std;

struct meshInfo {

	string name;
	std::vector<glm::vec3> verts;
	std::vector<glm::vec3> norms;
	std::vector<glm::vec3> tangent;
	std::vector<glm::vec3> biNormals;
	std::vector<glm::vec2> uvs;

	string diffuseTexture;
	glm::vec3 diffuse;
	float specular;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

};
std::vector<meshInfo*> meshes;

struct skelInfo {

	int animationIndex;

	int nrOfClusters;
	string jointName;
	glm::vec4 globalBindPose0;
	glm::vec4 globalBindPose1;
	glm::vec4 globalBindPose2;
	glm::vec4 globalBindPose3;

	glm::mat4 globalBindPosMat;

	std::vector<glm::vec4> transRow0;
	std::vector<glm::vec4> transRow1;
	std::vector<glm::vec4> transRow2;
	std::vector<glm::vec4> transRow3;

	std::vector<glm::mat4> transformMat;

	int nrOfKeys;
};
std::vector<skelInfo*> skeleton;

struct weights {
	int nrOfIndices;
	std::vector<int> indexPos;
	std::vector<glm::vec3> polygonVerteciesIndex;
	std::vector<glm::ivec4> controllers;
	std::vector<glm::vec4> weightsInfluence;
};
weights weightInfo;

glm::mat4 currentJointTrans[100];

GLuint sksBuff = 0;
GLuint animVerts;
GLuint animUVBuffer = 0;

int nrOfKeyframes = 0;
int currentFrame = 0;
float prog = 0;


bool readMesh(const char* filePath)
{
	glm::vec3 vec3;
	glm::vec2 vec2;
	ifstream in(filePath, ios::binary);
	int nrOfMeshes = 0;
	in.read(reinterpret_cast<char*>(&nrOfMeshes), sizeof(int));

	for (int i = 0; i < nrOfMeshes; i++) {
		//Get the name
		string name = "";
		int nrOfChars = 0;
		in.read(reinterpret_cast<char*>(&nrOfChars), sizeof(int));
		char *tempName;
		tempName = new char[nrOfChars];
		in.read(tempName, nrOfChars);
		//in.getline(tempName, nrOfChars + 1, '\0');
		name.append(tempName, nrOfChars);

		cout << name << endl;
		delete tempName;

		int nrOfCtrlPoints = 0;
		in.read(reinterpret_cast<char*>(&nrOfCtrlPoints), sizeof(int));

		cout << nrOfCtrlPoints << endl;

		meshInfo *mesh = new meshInfo;

		mesh->name = name;

		for (int k = 0; k < nrOfCtrlPoints; k++)
		{
			vec3 = glm::vec3(0);
			if (modelDebug)
			{
				cout << "Tell: " << in.tellg() << endl;
				cout << "Pos: " << endl;
			}
			//Read the Vertecies for the primitive
			for (int h = 0; h < 3; h++)
			{
				in.read(reinterpret_cast<char*>(&vec3), sizeof(vec3));
				if (modelDebug)
				{
					cout << "Vertex: ";
					cout << vec3.x << " ";
					cout << vec3.y << " ";
					cout << vec3.z << endl;
				}
				mesh->verts.push_back(vec3);
			}

			//Read the Normals for the primitive
			for (int h = 0; h < 3; h++) {
				in.read(reinterpret_cast<char*>(&vec3), sizeof(vec3));
				if (modelDebug)
				{
					cout << "Normal: ";
					cout << vec3.x << " ";
					cout << vec3.y << " ";
					cout << vec3.z << endl;
				}
				mesh->norms.push_back(vec3);

				//Read the Tangents for the primitive
				in.read(reinterpret_cast<char*>(&vec3), sizeof(vec3));
				if (modelDebug)
				{
					cout << "Tangent: ";
					cout << vec3.x << " ";
					cout << vec3.y << " ";
					cout << vec3.z << endl;
				}
				mesh->tangent.push_back(vec3);

				//Read the BiNormals for the primitive

				in.read(reinterpret_cast<char*>(&vec3), sizeof(vec3));
				if (modelDebug)
				{
					cout << "BiNormal: ";
					cout << vec3.x << " ";
					cout << vec3.y << " ";
					cout << vec3.z << endl;
				}
				mesh->biNormals.push_back(vec3);
			}

			//Read the UVs for the primitive
			for (int h = 0; h < 3; h++)
			{
				in.read(reinterpret_cast<char*>(&vec2), sizeof(vec2));
				if (modelDebug)
				{
					cout << "UV: ";
					cout << vec2.x << " ";
					cout << vec2.y << endl;
				}
				mesh->uvs.push_back(vec2);
			}
		}

		//Read the materials for the mesh as well as the Texture file name

		int fileNameLength = 0;
		glm::vec3 diffuse;
		float specular = 0;

		string diffuseTexture = "";
		in.read(reinterpret_cast<char*>(&fileNameLength), sizeof(int));
		char *tempFileName;
		tempFileName = new char[fileNameLength];
		in.read(tempFileName, fileNameLength);
		//in.getline(tempName, nrOfChars + 1, '\0');
		diffuseTexture.append(tempFileName, fileNameLength);

		in.read(reinterpret_cast<char*>(&diffuse), sizeof(diffuse));
		in.read(reinterpret_cast<char*>(&specular), sizeof(specular));

		mesh->diffuse = diffuse;
		mesh->specular = specular;

		vec3 = glm::vec3(0);
		//Read the position, rotation and scale values
		in.read(reinterpret_cast<char*>(&vec3), sizeof(vec3));
		mesh->position = vec3;
		in.read(reinterpret_cast<char*>(&vec3), sizeof(vec3));
		mesh->rotation = vec3;
		in.read(reinterpret_cast<char*>(&vec3), sizeof(vec3));
		mesh->scale = vec3;

		meshes.push_back(mesh);
	}

	return true;

}

void loadSkeleton(const char* filePath) {
	//First nrOfClusters
	ifstream in(filePath, ios::binary);
	int clusterNr = 0;

	int indexNr = 0;
	in.read(reinterpret_cast<char*>(&indexNr), sizeof(int));

	in.read(reinterpret_cast<char*>(&nrOfKeyframes), sizeof(int));

	in.read(reinterpret_cast<char*>(&clusterNr), sizeof(int));
	for (int i = 0; i < clusterNr; i++) {

		//Get the name
		string name = "";
		int nrOfChars = 0;
		in.read(reinterpret_cast<char*>(&nrOfChars), sizeof(int));
		char *tempName;
		tempName = new char[nrOfChars];
		in.read(tempName, nrOfChars);
		name.append(tempName, nrOfChars);

		cout << name << endl;
		delete tempName;

		skelInfo *info = new skelInfo;
		info->nrOfKeys = nrOfKeyframes;
		info->jointName = name;
		info->animationIndex = indexNr;
		glm::vec4 globalBindVec0;
		glm::vec4 globalBindVec1;
		glm::vec4 globalBindVec2;
		glm::vec4 globalBindVec3;

		in.read(reinterpret_cast<char*>(&globalBindVec0), sizeof(globalBindVec0));
		info->globalBindPose0 = globalBindVec0;

		in.read(reinterpret_cast<char*>(&globalBindVec1), sizeof(globalBindVec1));
		info->globalBindPose1 = globalBindVec1;

		in.read(reinterpret_cast<char*>(&globalBindVec2), sizeof(globalBindVec2));
		info->globalBindPose2 = globalBindVec2;

		in.read(reinterpret_cast<char*>(&globalBindVec3), sizeof(globalBindVec3));
		info->globalBindPose3 = globalBindVec3;

		glm::mat4 tempBindMat;

		tempBindMat = glm::mat4(info->globalBindPose0, info->globalBindPose1,
			info->globalBindPose2, info->globalBindPose3);

		info->globalBindPosMat = tempBindMat;

		for (int o = 0; o < nrOfKeyframes; o++) {

			glm::vec4 glmVec0;
			glm::vec4 glmVec1;
			glm::vec4 glmVec2;
			glm::vec4 glmVec3;
			in.read(reinterpret_cast<char*>(&glmVec0), sizeof(glmVec0));
			info->transRow0.push_back(glmVec0);
			in.read(reinterpret_cast<char*>(&glmVec1), sizeof(glmVec1));
			info->transRow1.push_back(glmVec1);
			in.read(reinterpret_cast<char*>(&glmVec2), sizeof(glmVec2));
			info->transRow2.push_back(glmVec2);
			in.read(reinterpret_cast<char*>(&glmVec3), sizeof(glmVec3));
			info->transRow3.push_back(glmVec3);

			glm::mat4 tempMap = glm::mat4(glmVec0, glmVec1, glmVec2, glmVec3);
			info->transformMat.push_back(tempMap);

		}
		skeleton.push_back(info);
	}

}

void loadWeight(const char* filePath) {

	ifstream in(filePath, ios::binary);

	int nrOfIndices = 0;
	int nrOfPolygons = 0;
	in.read(reinterpret_cast<char*>(&nrOfPolygons), sizeof(int));
	in.read(reinterpret_cast<char*>(&nrOfIndices), sizeof(int));
	weightInfo.nrOfIndices = nrOfIndices;
	int polygonIndex[3];
	int jointIndex = 0;
	float influence = 0;

	glm::ivec3 polygonVertexIndex;
	glm::ivec4 controllers;
	glm::vec4 weightInfluences;

	for (int k = 0; k < nrOfPolygons; k++) {

		for (int i = 0; i < 3; i++) {
			in.read(reinterpret_cast<char*>(&polygonIndex[i]), sizeof(int));
			polygonVertexIndex[i] = polygonIndex[i];
		}
		weightInfo.polygonVerteciesIndex.push_back(polygonVertexIndex);
		weightInfo.indexPos.push_back(k);

		for (int i = 0; i < 3; i++) {

			for (int q = 0; q < 4; q++) {
				int check = in.tellg();
				in.read(reinterpret_cast<char*>(&jointIndex), sizeof(int));
				controllers[q] = jointIndex;

				in.read(reinterpret_cast<char*>(&influence), sizeof(influence));
				weightInfluences[q] = influence;
				check = in.tellg();

			}
			weightInfo.controllers.push_back(controllers);
			weightInfo.weightsInfluence.push_back(weightInfluences);
		}
	}
}

void updateAnim(int animIndex) {

	if (currentFrame < skeleton[0]->nrOfKeys) {
		currentFrame++;
	}
	else {
		currentFrame = 1;
	}

	for (int i = 0; i < skeleton.size(); i++) {

		currentJointTrans[i] = skeleton[i]->transformMat[currentFrame - 1] * skeleton[i]->globalBindPosMat;
	}
}