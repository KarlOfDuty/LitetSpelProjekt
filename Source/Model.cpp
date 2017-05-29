#include "Model.h"
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))
//Returns the index of a material in the vector matching provided name, -1 if not found
int Material::findMaterial(std::string name, std::vector<Material> materials)
{
	int index = -1;
	for (int i = 0; i < materials.size() && index == -1; i++)
	{
		if (materials.at(i).name == name)
		{
			index = i;
		}
	}
	return index;
}
//Getters
glm::mat4 Model::getModelMatrix() const
{
	return this->modelMatrix;
}
glm::mat4 Model::getRotationMatrix() const
{
	return this->rotationMatrix;
}
void Model::getMinMaxBouding(glm::vec3 &min, glm::vec3 &max)
{
	if (minBounding == glm::vec3() && maxBounding == glm::vec3())
	{
		for (int i = 0; i < meshes.size(); i++)
		{
			for (int j = 0; j < meshes[i]->vertices.size(); j++)
			{
				if (meshes[i]->vertices[j].pos.x < minBounding.x) minBounding.x = meshes[i]->vertices[j].pos.x;
				if (meshes[i]->vertices[j].pos.y < minBounding.y) minBounding.y = meshes[i]->vertices[j].pos.y;
				if (meshes[i]->vertices[j].pos.x > maxBounding.x) maxBounding.x = meshes[i]->vertices[j].pos.x;
				if (meshes[i]->vertices[j].pos.y > maxBounding.y) maxBounding.y = meshes[i]->vertices[j].pos.y;
			}
		}
	}
	min = minBounding;
	max = maxBounding;
}
void Model::getScaledMinMaxBouding(glm::vec3 &min, glm::vec3 &max)
{
	if (minBounding == glm::vec3() && maxBounding == glm::vec3())
	{
		for (int i = 0; i < meshes.size(); i++)
		{
			for (int j = 0; j < meshes[i]->vertices.size(); j++)
			{
				if (meshes[i]->vertices[j].pos.x < minBounding.x) minBounding.x = meshes[i]->vertices[j].pos.x;
				if (meshes[i]->vertices[j].pos.y < minBounding.y) minBounding.y = meshes[i]->vertices[j].pos.y;
				if (meshes[i]->vertices[j].pos.x > maxBounding.x) maxBounding.x = meshes[i]->vertices[j].pos.x;
				if (meshes[i]->vertices[j].pos.y > maxBounding.y) maxBounding.y = meshes[i]->vertices[j].pos.y;
			}
		}
	}
	glm::vec3 scale;
	glm::quat rotation;
	glm::decompose(modelMatrix, scale, rotation, glm::vec3(), glm::vec3(), glm::vec4());
	min = minBounding * scale;
	max = maxBounding * scale;
	if (rotation.z != 0)
	{
		min *= 2.f;
		max *= 2.f;
	}
}
void Model::getScaledMinMaxBouding(glm::vec3 &min, glm::vec3 &max, glm::mat4 modelMat)
{
	if (minBounding == glm::vec3() && maxBounding == glm::vec3())
	{
		for (int i = 0; i < meshes.size(); i++)
		{
			for (int j = 0; j < meshes[i]->vertices.size(); j++)
			{
				if (meshes[i]->vertices[j].pos.x < minBounding.x) minBounding.x = meshes[i]->vertices[j].pos.x;
				if (meshes[i]->vertices[j].pos.y < minBounding.y) minBounding.y = meshes[i]->vertices[j].pos.y;
				if (meshes[i]->vertices[j].pos.x > maxBounding.x) maxBounding.x = meshes[i]->vertices[j].pos.x;
				if (meshes[i]->vertices[j].pos.y > maxBounding.y) maxBounding.y = meshes[i]->vertices[j].pos.y;
			}
		}
	}
	glm::vec3 scale;
	glm::decompose(modelMat, scale, glm::quat(), glm::vec3(), glm::vec3(), glm::vec4());
	min = minBounding * scale;
	max = maxBounding * scale;
}
Material Model::getMaterial(int index)
{
	return this->meshes.at(index)->material;
}
std::vector<glm::vec2> Model::getPoints()
{
	if (allPoints.empty())
	{
		glm::vec3 min, max;
		getMinMaxBouding(min, max);
		//Pushback points without rotation
		allPoints.push_back(glm::vec2(min));
		allPoints.push_back(glm::vec2(min.x, max.y));
		allPoints.push_back(glm::vec2(max));
		allPoints.push_back(glm::vec2(max.x, min.y));

	}

	std::vector<glm::vec2> translatedPoint;
	//Get rotation and scale from modelMat
	glm::vec3 scale;
	glm::quat rotation;
	glm::decompose(this->modelMatrix, scale, rotation, glm::vec3(), glm::vec3(), glm::vec4());

	//Convert from quat to radians
	double t3 = +2.0 * (rotation.w * rotation.z + rotation.x * rotation.y);
	double t4 = +1.0 - 2.0f * ((rotation.y * rotation.y) + rotation.z * rotation.z);
	float radians = -std::atan2(t3, t4);

	//Pushback points without rotation
	translatedPoint.push_back(glm::vec2(allPoints[0].x*scale.x, allPoints[0].y*scale.y));
	translatedPoint.push_back(glm::vec2(allPoints[1].x*scale.x, allPoints[1].y*scale.y));
	translatedPoint.push_back(glm::vec2(allPoints[2].x*scale.x, allPoints[2].y*scale.y));
	translatedPoint.push_back(glm::vec2(allPoints[3].x*scale.x, allPoints[3].y*scale.y));

	//Translate to right position depending on rotation
	for (int k = 0; k < translatedPoint.size(); k++)
	{
		glm::vec2 center = this->modelMatrix[3];
		translatedPoint[k] += center;
		float x = center.x + (translatedPoint[k].x - center.x) * cos(radians) - (translatedPoint[k].y - center.y) * sin(radians);
		float y = center.y + (translatedPoint[k].x - center.x) * sin(radians) + (translatedPoint[k].y - center.y) * cos(radians);

		translatedPoint[k].x = x;
		translatedPoint[k].y = y;
	}

	return translatedPoint;
}
glm::vec3 Model::getPos() const
{
	return modelMatrix[3];
}
std::string Model::type() const
{
	return "Model";
}
float Model::getBoundingSphereRadius() const
{
	return boundingSphereRadius;
}
//Setters
void Model::setModelMatrix(glm::mat4 modelMat)
{
	this->modelMatrix = modelMat;
}
void Model::setRotationMatrix(glm::mat4 rotationMat)
{
	this->rotationMatrix = rotationMat;
}
void Model::setRotationMatrix(glm::vec3 rotation)
{
	rotation = rotation * 3.14f / 180.0f;
	glm::quat quaternion = glm::quat(rotation);
	
	//This might not work depending on exporter values
	this->rotationMatrix = glm::toMat4(quaternion);
}
void Model::setPos(glm::vec3 pos)
{
	this->modelMatrix[3] = glm::vec4(pos,1.0);
}
//Probably only safe to use this before any rotations have been made
void Model::setScale(glm::vec3& scale)
{
	this->modelMatrix[0][0] = scale.x;
	this->modelMatrix[1][1] = scale.y;
	this->modelMatrix[2][2] = scale.z;
}
void Model::addMesh(Mesh* mesh)
{
	meshes.push_back(mesh);
}
//Sets the radius of the bounding sphere around this model
void Model::setBoundingSphereRadius()
{
	//Takes the distance to the furthest away vertex and sets it as the radius
	float radius = 0.0f;
	for (int i = 0; i < meshes.size(); i++)
	{
		for (int j = 0; j < meshes[i]->vertices.size(); j++)
		{
			radius = glm::max(radius, (float)meshes[i]->vertices[j].pos.length());
		}
	}
	this->boundingSphereRadius = radius;
}
//Multiplies the model matrix with the rotation matrix
void Model::rotate()
{
	this->modelMatrix *= rotationMatrix;
}
//Reads a .obj file and creates a Model object from the data
void Model::readOBJ(std::string filename)
{
	//Temporary containers
	std::ifstream file(filename);
	std::string str = "";
	//Vector of all vertex positions
	std::vector<glm::vec3> vertexPos = std::vector<glm::vec3>();
	vertexPos.push_back(glm::vec3(0, 0, 0));
	//All vertex normals
	std::vector<glm::vec3> vertexNormals = std::vector<glm::vec3>();
	vertexNormals.push_back(glm::vec3(0, 0, 0));
	//All texture coordinates
	std::vector<glm::vec2> vertexTex = std::vector<glm::vec2>();
	vertexTex.push_back(glm::vec3(0, 0, 0));
	//Current material file's materials
	std::vector<Material> materials = std::vector<Material>();
	Material currentMaterial = Material();
	std::vector<Vertex> meshVertices = std::vector<Vertex>();
	Mesh* mesh = new Mesh();

	//Gets a single line of the file at a time
	while (std::getline(file, str))
	{
		std::stringstream line;
		float data;
		//Takes the first word of the line and compares it to variable names
		line << str;
		line >> str;
		if (str == "v")
		{
			//A vertex position
			glm::vec3 aVertexPos;
			if (modelDebug)std::cout << "Vertex (v): ";
			//X
			line >> data;
			aVertexPos.x = data;
			if (modelDebug)std::cout << data << " ";
			//Y
			line >> data;
			aVertexPos.y = data;
			if (modelDebug)std::cout << data << " ";
			//Z
			line >> data;
			aVertexPos.z = data;
			if (modelDebug)std::cout << data << " ";

			vertexPos.push_back(aVertexPos);
			if (modelDebug)std::cout << std::endl;
		}
		else if (str == "vt")
		{
			//A texture position
			glm::vec2 aVertexTex;
			if (modelDebug)std::cout << "Texture Position (vt): ";
			//U
			line >> data;
			aVertexTex.x = data;
			if (modelDebug)std::cout << data << " ";
			//V
			line >> data;
			aVertexTex.y = data;
			if (modelDebug)std::cout << data << " ";

			vertexTex.push_back(aVertexTex);
			if (modelDebug)std::cout << std::endl;
		}
		else if (str == "vn")
		{
			//A normal
			glm::vec3 normal;
			if (modelDebug)std::cout << "Normal (vn): ";
			//X
			line >> data;
			normal.x = data;
			if (modelDebug)std::cout << data << " ";
			//Y
			line >> data;
			normal.y = data;
			if (modelDebug)std::cout << data << " ";
			//Z
			line >> data;
			normal.z = data;
			if (modelDebug)std::cout << data << " ";

			vertexNormals.push_back(normal);
			if (modelDebug)std::cout << std::endl;
		}
		else if (str == "f")
		{
			//Faces
			if (modelDebug)std::cout << "Face (f): ";
			std::vector<Vertex> aFace = std::vector<Vertex>();
			//Split the rest of the line into seperate words
			while (line >> str)
			{
				std::stringstream strIndices;
				strIndices << str;
				if (modelDebug)std::cout << str << " ";
				int i = 0;
				std::stringstream intIndices;
				//Splits up the indices to be seperated by spaces instead of slashes.
				while (std::getline(strIndices, str, '/'))
				{
					intIndices << str;
					intIndices << " ";
					//Count number of indices to identify either v/vt/vn or v//vn format
					i++;
				};
				Vertex aVertex;
				//Creates a vertex from the data pointed to by the indices
				if (i == 3)
				{
					intIndices >> i;
					aVertex.pos = vertexPos.at(i);
					intIndices >> i;
					aVertex.texPos = vertexTex.at(i);
					intIndices >> i;
					aVertex.normal = vertexNormals.at(i);
				}
				else if (i == 2)
				{
					strIndices >> i;
					aVertex.pos = vertexPos.at(i);
					strIndices >> i;
					aVertex.normal = vertexNormals.at(i);
				}
				//Adds the vertex to this Mesh
				meshVertices.push_back(aVertex);
			}
			if (modelDebug)std::cout << std::endl;
		}
		else if (str == "g" || str == "o")
		{
			//Groups
			if (modelDebug)std::cout << "Group name (g): ";
			while (line >> str)
			{
				if (modelDebug)std::cout << str << " ";
				//Finishes the previous mesh if this is not the first one
				if (!meshVertices.empty())
				{
					mesh->vertices = meshVertices;
					mesh->material = currentMaterial;
					meshes.push_back(mesh);
					mesh = new Mesh();
					meshVertices = std::vector<Vertex>();
				}
			}
			if (modelDebug)std::cout << std::endl;
		}
		else if (str == "mtllib")
		{
			//Material library
			line >> str;
			if (modelDebug)std::cout << std::endl << "Material Library (mtllib): " << str << std::endl;
			std::string filePath = filename.substr(0, filename.find_last_of("\\/") + 1);
			std::ifstream mtlFile(filePath + str);
			//Index of material currently being added
			int materialBeingAdded = -1;
			while (std::getline(mtlFile, str))
			{
				//Takes the first word of the line and compares it to variable names
				std::stringstream mtlWord;
				mtlWord << str;
				mtlWord >> str;
				if (str == "newmtl")
				{
					//Check if the material already exists to avoid duplicates
					mtlWord >> str;
					int foundMaterial = Material::findMaterial(str, materials);
					if (foundMaterial == -1)
					{
						//Create new material and enter material name
						if (matDebug)std::cout << "Material name: " << str << std::endl;
						materialBeingAdded = (int)materials.size();
						materials.push_back(Material());
						materials.at(materialBeingAdded).name = str;
					}
					else
					{
						//Material is already added and will be skipped
						materialBeingAdded = -1;
					}
				}
				else if (str == "Ka" && materialBeingAdded != -1)
				{
					if (matDebug)std::cout << "Ambient colour: ";
					float data = 0.0;
					//R
					mtlWord >> data;
					if (matDebug)std::cout << data << " ";
					materials.at(materialBeingAdded).ambientColour.x = data;
					//G
					mtlWord >> data;
					if (matDebug)std::cout << data << " ";
					materials.at(materialBeingAdded).ambientColour.y = data;
					//B
					mtlWord >> data;
					if (matDebug)std::cout << data << " ";
					materials.at(materialBeingAdded).ambientColour.z = data;
					if (matDebug)std::cout << std::endl;
				}
				else if (str == "Kd" && materialBeingAdded != -1)
				{
					if (matDebug)std::cout << "Diffuse colour: ";
					float data = 0.0;
					//R
					mtlWord >> data;
					if (matDebug)std::cout << data << " ";
					materials.at(materialBeingAdded).diffuseColour.x = data;
					//G
					mtlWord >> data;
					if (matDebug)std::cout << data << " ";
					materials.at(materialBeingAdded).diffuseColour.y = data;
					//B
					mtlWord >> data;
					if (matDebug)std::cout << data << " ";
					materials.at(materialBeingAdded).diffuseColour.z = data;
					if (matDebug)std::cout << std::endl;
				}
				else if (str == "Ks" && materialBeingAdded != -1)
				{
					if (matDebug)std::cout << "Specular colour: ";
					float data = 0.0;
					//R
					mtlWord >> data;
					if (matDebug)std::cout << data << " ";
					materials.at(materialBeingAdded).specularColour.x = data;
					//G
					mtlWord >> data;
					if (matDebug)std::cout << data << " ";
					materials.at(materialBeingAdded).specularColour.y = data;
					//B
					mtlWord >> data;
					if (matDebug)std::cout << data << " ";
					materials.at(materialBeingAdded).specularColour.z = data;
					if (matDebug)std::cout << std::endl;
				}
				else if (str == "map_Ka" && materialBeingAdded != -1)
				{
					//Name of the file containing the ambient texture map
					mtlWord >> str;
					if (matDebug)std::cout << "Ambient texture map: " << filePath + str << std::endl;
					materials.at(materialBeingAdded).textureMapAmbientFile = filePath + str;
				}
				else if (str == "map_Kd" && materialBeingAdded != -1)
				{
					//Name of the file containing the diffuse texture map
					mtlWord >> str;
					if (matDebug)std::cout << "Diffuse texture map: " << filePath + str << std::endl;
					materials.at(materialBeingAdded).textureMapDiffuseFile = filePath + str;
				}
				else if (str == "map_Ks" && materialBeingAdded != -1)
				{
					//Name of the file containing the specular texture map
					mtlWord >> str;
					if (matDebug)std::cout << "Specular texture map: " << filePath + str << std::endl;
					materials.at(materialBeingAdded).textureMapSpecularFile = filePath + str;
				}
				else if ((str == "map_bump" || str == "bump" || str == "norm") && materialBeingAdded != -1)
				{
					//Name of the file containing the normal map
					mtlWord >> str;
					if (matDebug)std::cout << "Normal/Bump map: " << filePath + str << std::endl;
					materials.at(materialBeingAdded).normalMapFile = filePath + str;
				}
			}
			if (matDebug)std::cout << std::endl;
		}
		else if (str == "usemtl")
		{
			//Material name
			if (modelDebug)std::cout << "Material name (usemtl): ";
			while (line >> str)
			{
				//Set the current material so it can be assigned to faces
				int index = Material::findMaterial(str, materials);
				if (index != -1)
				{
					currentMaterial = materials.at(index);
				}
				if (modelDebug)std::cout << str << " ";
			}
			if (modelDebug)std::cout << std::endl;
		}
	}
	//Finishes the last mesh
	if (!meshVertices.empty())
	{
		mesh->vertices = meshVertices;
		mesh->material = currentMaterial;
		meshes.push_back(mesh);
		mesh = new Mesh();
		meshVertices = std::vector<Vertex>();
	}
}
bool Model::readModel(const char* filePath)
{
	glm::vec3 vec3;
	glm::vec2 vec2;
	std::ifstream in(filePath, std::ios::binary);
	int nrOfMeshes = 0;
	in.read(reinterpret_cast<char*>(&nrOfMeshes), sizeof(int));
	//Read the name of the mesh
	std::string name = "";
	int nrOfChars = 0;
	in.read(reinterpret_cast<char*>(&nrOfChars), sizeof(int));
	char *tempName;
	tempName = new char[nrOfChars];
	in.read(tempName, nrOfChars);
	name.append(tempName, nrOfChars);
	if (modelDebug)std::cout << name << std::endl;
	delete[] tempName;

	int nrOfCtrlPoints = 0;
	in.read(reinterpret_cast<char*>(&nrOfCtrlPoints), sizeof(int));

	if (modelDebug)std::cout << nrOfCtrlPoints << std::endl;
	Mesh *mesh = new Mesh();
	mesh->name = name;

	for (int k = 0; k < nrOfCtrlPoints; k++)
	{
		vec3 = glm::vec3(0);
		if (modelDebug)
		{
			std::cout << "Tell: " << in.tellg() << std::endl;
			std::cout << "Pos: " << std::endl;
		}
		//Read the Vertices for the primitive
		for (int h = 0; h < 3; h++)
		{
			mesh->vertices.push_back(Vertex());
			in.read(reinterpret_cast<char*>(&vec3), sizeof(vec3));
			if (modelDebug)
			{
				std::cout << "Vertex: ";
				std::cout << vec3.x << " ";
				std::cout << vec3.y << " ";
				std::cout << vec3.z << std::endl;
			}
			mesh->vertices[(k * 3) + h].pos = vec3;
		}
		for (int h = 0; h < 3; h++)
		{
			//Read the Normals for the primitive
			in.read(reinterpret_cast<char*>(&vec3), sizeof(vec3));
			if (modelDebug)
			{
				std::cout << "Normal: ";
				std::cout << vec3.x << " ";
				std::cout << vec3.y << " ";
				std::cout << vec3.z << std::endl;
			}
			mesh->vertices[(k * 3) + h].normal = vec3;
			//Read the Tangents for the primitive
			in.read(reinterpret_cast<char*>(&vec3), sizeof(vec3));
			if (modelDebug)
			{
				std::cout << "Tangent: ";
				std::cout << vec3.x << " ";
				std::cout << vec3.y << " ";
				std::cout << vec3.z << std::endl;
			}
			mesh->vertices[(k * 3) + h].tangent = vec3;
			//Read the BiNormals for the primitive
			in.read(reinterpret_cast<char*>(&vec3), sizeof(vec3));
			if (modelDebug)
			{
				std::cout << "BiNormal: ";
				std::cout << vec3.x << " ";
				std::cout << vec3.y << " ";
				std::cout << vec3.z << std::endl;
			}
			mesh->vertices[(k * 3) + h].biTangent = vec3;
		}
		//Read the UVs for the primitive
		for (int h = 0; h < 3; h++)
		{
			in.read(reinterpret_cast<char*>(&vec2), sizeof(vec2));
			if (modelDebug)
			{
				std::cout << "UV: ";
				std::cout << vec2.x << " ";
				std::cout << vec2.y << std::endl;
			}
			mesh->vertices[(k * 3) + h].texPos = vec2;
		}
	}
	//Diffuse texture file
	int fileNameLength = 0;
	in.read(reinterpret_cast<char*>(&fileNameLength), sizeof(int));
	if (fileNameLength)
	{
		std::string diffuseFileName = "";
		char *tempFileName = new char[fileNameLength];
		in.read(tempFileName, fileNameLength);
		diffuseFileName.append(tempFileName, fileNameLength);
		if (diffuseFileName != "NULL")
		{
			std::cout << "'" << diffuseFileName << "'" << std::endl;
			mesh->material.textureMapDiffuseFile = diffuseFileName;
		}
		delete[] tempFileName;
	}
	//Diffuse colour
	glm::vec3 diffuseColour;
	in.read(reinterpret_cast<char*>(&diffuseColour), sizeof(diffuseColour));
	mesh->material.diffuseColour = glm::vec3(0.5, 0.5, 0.5);
	mesh->material.diffuseColour = diffuseColour;
	//Specularity
	float specularity = 0;
	in.read(reinterpret_cast<char*>(&specularity), sizeof(specularity));
	mesh->material.specularColour = glm::vec3(specularity, specularity, specularity);
	//Not used
	mesh->material.ambientColour = glm::vec3(0.5, 0.5, 0.5);
	//Position
	glm::vec3 pos;
	in.read(reinterpret_cast<char*>(&pos), sizeof(pos));
	this->setPos(pos);
	//Rotation
	glm::vec3 rotation;
	in.read(reinterpret_cast<char*>(&rotation), sizeof(rotation));
	this->setRotationMatrix(rotation);
	//Scale
	glm::vec3 scale;
	in.read(reinterpret_cast<char*>(&scale), sizeof(scale));
	this->setScale(scale);
	//Set up model
	this->rotate();
	this->addMesh(mesh);
	this->setupModel();
	this->loadTextures(0);
	this->setBoundingSphereRadius();
	in.close();
	return true;
}
void Model::loadSkeleton(const char* filePath)
{
	//First nrOfClusters
	std::ifstream in(filePath, std::ios::binary);
	int clusterNr = 0;

	int indexNr = 0;
	in.read(reinterpret_cast<char*>(&indexNr), sizeof(int));
	in.read(reinterpret_cast<char*>(&nrOfKeyframes), sizeof(int));
	in.read(reinterpret_cast<char*>(&clusterNr), sizeof(int));
	for (int i = 0; i < clusterNr; i++) 
	{
		//Get the name
		std::string name = "";
		int nrOfChars = 0;
		in.read(reinterpret_cast<char*>(&nrOfChars), sizeof(int));
		char *tempName;
		tempName = new char[nrOfChars];
		in.read(tempName, nrOfChars);
		name.append(tempName, nrOfChars);

		//std::cout << name << std::endl;
		delete[] tempName;

		Joint *joint = new Joint();
		joint->nrOfKeys = nrOfKeyframes;
		joint->jointName = name;
		joint->animationIndex = indexNr;

		in.read(reinterpret_cast<char*>(&joint->globalBindPosMat[0]), sizeof(joint->globalBindPosMat[0]));
		in.read(reinterpret_cast<char*>(&joint->globalBindPosMat[1]), sizeof(joint->globalBindPosMat[1]));
		in.read(reinterpret_cast<char*>(&joint->globalBindPosMat[2]), sizeof(joint->globalBindPosMat[2]));
		in.read(reinterpret_cast<char*>(&joint->globalBindPosMat[3]), sizeof(joint->globalBindPosMat[3]));

		for (int o = 0; o < nrOfKeyframes; o++)
		{
			glm::mat4 tempMap;
			in.read(reinterpret_cast<char*>(&tempMap[0]), sizeof(tempMap[0]));
			in.read(reinterpret_cast<char*>(&tempMap[1]), sizeof(tempMap[1]));
			in.read(reinterpret_cast<char*>(&tempMap[2]), sizeof(tempMap[2]));
			in.read(reinterpret_cast<char*>(&tempMap[3]), sizeof(tempMap[3]));
			joint->transformMat.push_back(tempMap);
		}
		this->skeleton.push_back(joint);
		hasAnimations = true;
	}
}
void Model::loadWeight(const char* filePath)
{

	std::ifstream in(filePath, std::ios::binary);

	int nrOfIndices = 0;
	int nrOfPolygons = 0;
	in.read(reinterpret_cast<char*>(&nrOfPolygons), sizeof(int));
	in.read(reinterpret_cast<char*>(&nrOfIndices), sizeof(int));
	int polygonIndex[3];
	int jointIndex = 0;
	float influence = 0;

	glm::ivec4 controllers;
	glm::vec4 weightInfluences;

	for (int k = 0; k < nrOfPolygons; k++) 
	{
		for (int i = 0; i < 3; i++) 
		{
			in.read(reinterpret_cast<char*>(&polygonIndex[i]), sizeof(int));
		}

		for (int i = 0; i < 3; i++)
		{
			for (int q = 0; q < 4; q++)
			{
				int check = in.tellg();
				in.read(reinterpret_cast<char*>(&jointIndex), sizeof(int));
				controllers[q] = jointIndex;

				in.read(reinterpret_cast<char*>(&influence), sizeof(influence));
				weightInfluences[q] = influence;
				check = in.tellg();

			}
			meshes[0]->vertices[(k*3) + i].controllers = controllers;
			meshes[0]->vertices[(k * 3) + i].weightsInfluence = weightInfluences;
		}
	}
}
void Model::updateAnimation()
{
	if (skeleton.size())
	{
		if (currentFrame < this->skeleton[0]->nrOfKeys-1)
		{
			currentFrame++;
		}
		else
		{
			currentFrame = 1;
		}
	}
}
//Draws the model
void Model::draw(Shader shader)
{
	if (skeleton.size())
	{
		//std::cout << this->skeleton[0]->transformMat.size() << " " << currentFrame << std::endl;
		for (int i = 0; i < skeleton.size(); i++)
		{
			currentJointTrans[i] = skeleton[i]->globalBindPosMat;
		}
		for (int j = 0; j < 100; j++)
		{
			glUniformMatrix4fv(glGetUniformLocation(shader.program, ("currentJointTrans[" + std::to_string(j) + "]").c_str()), 1, GL_FALSE, glm::value_ptr(currentJointTrans[j]));
		}
	}
	glUniform1i(glGetUniformLocation(shader.program,"hasAnimation"),hasAnimations);
	//Draw vertices
	glBindVertexArray(this->VAO);
	for (int i = 0; i < this->meshes.size(); i++)
	{
		//Ambient Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, meshes[i]->material.ambientTexture);
		glUniform1i(glGetUniformLocation(shader.program, "ambientTexture"), 0);
		//Diffuse Texture
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, meshes[i]->material.diffuseTexture);
		glUniform1i(glGetUniformLocation(shader.program, "diffuseTexture"), 1);
		//Specular Texture
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, meshes[i]->material.specularTexture);
		glUniform1i(glGetUniformLocation(shader.program, "specularTexture"), 2);
		//Normal Map
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, meshes[i]->material.normalMapTexture);
		glUniform1i(glGetUniformLocation(shader.program, "normalMap"), 3);
		
		glDrawArrays(GL_TRIANGLES, 0, (int)this->meshes[i]->vertices.size()*3);
	}
	glBindVertexArray(0);
}
//Sets the model up to be drawn
void Model::setupModel()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	std::vector<Vertex> vertices = std::vector<Vertex>();
	for (int i = 0; i < meshes.size(); i++)
	{
		//Iterate through vertices in the face
		for (int j = 0; j < meshes[i]->vertices.size(); j++)
		{
			vertices.push_back(meshes[i]->vertices.at(j));
		}
		loadTextures(i);
	}
	glBufferData(GL_ARRAY_BUFFER, 
		vertices.size() * sizeof(Vertex), 
		&vertices[0], GL_STATIC_DRAW);
	//Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));
	//Texture Coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 3));
	//Normal
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 5));
	//Weights
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 8));
	//Controllers
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_INT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 12));
	//Unbind the vertex array buffer
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
//Loads textures from memory, not from disk
void Model::loadTextures(int i)
{
	//Loading ambient texture or colour for mesh
	glGenTextures(1, &meshes[i]->material.ambientTexture);
	glBindTexture(GL_TEXTURE_2D, meshes[i]->material.ambientTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//Check weather to use texture or solid colour
	if (meshes[i]->material.textureMapAmbientFile != "")
	{
		int width, height;
		unsigned char* image;
		image = SOIL_load_image(meshes[i]->material.textureMapAmbientFile.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		float colour[3] = {
			meshes[i]->material.ambientColour.r,
			meshes[i]->material.ambientColour.g,
			meshes[i]->material.ambientColour.b
		};
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_FLOAT, colour);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	//Loading diffuse texture or colour for mesh
	glGenTextures(1, &meshes[i]->material.diffuseTexture);
	glBindTexture(GL_TEXTURE_2D, meshes[i]->material.diffuseTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//Check weather to use texture or solid colour
	if (meshes[i]->material.textureMapDiffuseFile != "")
	{	
		int width, height;
		unsigned char* image;
		image = SOIL_load_image(meshes[i]->material.textureMapDiffuseFile.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		float colour[3] = { 
			meshes[i]->material.diffuseColour.r,
			meshes[i]->material.diffuseColour.g,
			meshes[i]->material.diffuseColour.b
		};
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_FLOAT, colour);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	//Loading specular texture or colour for mesh
	glGenTextures(1, &meshes[i]->material.specularTexture);
	glBindTexture(GL_TEXTURE_2D, meshes[i]->material.specularTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//Check weather to use texture or solid colour
	if (meshes[i]->material.textureMapSpecularFile != "")
	{
		int width, height;
		unsigned char* image;
		image = SOIL_load_image(meshes[i]->material.textureMapSpecularFile.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
	}
	else
	{
		float colour[3] = { 
			meshes[i]->material.specularColour.r,
			meshes[i]->material.specularColour.g,
			meshes[i]->material.specularColour.b
		};
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_FLOAT, colour);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}
//Constructors
Model::Model(std::string filename)
{
	//Initializes the model without a rotation or model matrix. Does not set the model up so it can be drawn.
	this->hasAnimations = false;
	this->modelMatrix = glm::mat4(1.0);
	this->rotationMatrix = glm::mat4(1.0);
	readOBJ(filename);
	setupModel();
	setBoundingSphereRadius();
}
Model::Model(std::string filename, glm::mat4 modelMat)
{
	//Initializes the model without a rotation
	this->hasAnimations = false;
	this->modelMatrix = modelMat;
	this->rotationMatrix = glm::mat4(1.0);
	readOBJ(filename);
	setupModel();
	setBoundingSphereRadius();
}
Model::Model(std::string filename, glm::mat4 modelMat, glm::mat4 rotation)
{
	//Initializes the model
	this->hasAnimations = false;
	this->modelMatrix = modelMat;
	this->rotationMatrix = rotation;
	readOBJ(filename);
	setupModel();
	setBoundingSphereRadius();
}
Model::Model()
{
	//Initializes the model with no data
	this->hasAnimations = false;
	this->modelMatrix = glm::mat4(1.0);
	this->rotationMatrix = glm::mat4(1.0);
}
//Copy constructors
Model::Model(Model &otherModel)
{
	this->hasAnimations = otherModel.hasAnimations;
	this->modelMatrix = otherModel.modelMatrix;
	this->rotationMatrix = otherModel.rotationMatrix;
	this->meshes = otherModel.meshes;
	this->VAO = otherModel.VAO;
	this->VBO = otherModel.VBO;
	this->boundingSphereRadius = otherModel.boundingSphereRadius;
}
Model::Model(Model *otherModel)
{
	this->hasAnimations = otherModel->hasAnimations;
	this->modelMatrix = otherModel->modelMatrix;
	this->rotationMatrix = otherModel->rotationMatrix;
	this->meshes = otherModel->meshes;
	this->VAO = otherModel->VAO;
	this->VBO = otherModel->VBO;
	this->boundingSphereRadius = otherModel->boundingSphereRadius;
}
Model::Model(Model &otherModel, glm::mat4 modelMat)
{
	this->hasAnimations = otherModel.hasAnimations;
	this->modelMatrix =  modelMat;
	this->rotationMatrix = otherModel.rotationMatrix;
	this->meshes = otherModel.meshes;
	this->VAO = otherModel.VAO;
	this->VBO = otherModel.VBO;
	this->boundingSphereRadius = otherModel.boundingSphereRadius;
}
Model::Model(Model *otherModel, glm::mat4 modelMat)
{
	this->hasAnimations = otherModel->hasAnimations;
	this->modelMatrix = modelMat;
	this->rotationMatrix = otherModel->rotationMatrix;
	this->meshes = otherModel->meshes;
	this->VAO = otherModel->VAO;
	this->VBO = otherModel->VBO;
	this->boundingSphereRadius = otherModel->boundingSphereRadius;
}
//Destructor
Model::~Model()
{
	meshes.clear();
}
//As models share pointers this can not be done in the destructor
void Model::deleteMeshes()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		delete meshes[i];
	}
	meshes.clear();
}