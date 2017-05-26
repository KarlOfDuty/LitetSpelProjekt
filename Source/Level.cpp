#include "Level.h"

//Loads a single copy of all obj models used into memory
void Level::loadModels()
{
	//for (int i = 0; i < modelFilePaths.size(); i++)
	//{
	//	modelLibrary.push_back(new Model(modelFilePaths[i]));
	//}
}
void Level::loadLevel()
{
	//Temporary containers
	std::ifstream file(filePath);
	std::string str = "";

	//Gets a single line of the file at a time
	while (std::getline(file, str))
	{
		std::stringstream line;
		std::string path;
		//Takes the first word of the line and compares it to variable names
		line << str;
		line >> str;
		std::cout << str << std::endl;
		if (str == "staticModels")
		{
			line >> path;
			//readModels(path.c_str(), staticModels);
		}
		else if (str == "dynamicModels")
		{
			line >> path;
			readModels(path.c_str(), dynamicModels);
		}
		else if (str == "colliders")
		{
			line >> path;
			readModels(path.c_str(), colliders);
			if(showColliders)readModels(path.c_str(), staticModels);
		}
		else if (str == "triggers")
		{
			line >> path;
			//readModels(path.c_str(), staticModels);
		}
	}
}
bool Level::readModels(const char* filePath, std::vector<Model*> &modelVector)
{
	glm::vec3 vec3;
	glm::vec2 vec2;
	std::ifstream in(filePath, std::ios::binary);
	int nrOfMeshes = 0;
	in.read(reinterpret_cast<char*>(&nrOfMeshes), sizeof(int));

	for (int i = 0; i < nrOfMeshes; i++)
	{
		//Get the name
		std::string name = "";
		int nrOfChars = 0;
		in.read(reinterpret_cast<char*>(&nrOfChars), sizeof(int));
		char *tempName;
		tempName = new char[nrOfChars];
		in.read(tempName, nrOfChars);
		//in.getline(tempName, nrOfChars + 1, '\0');
		name.append(tempName, nrOfChars);

		if (modelDebug)std::cout << name << std::endl;
		delete tempName;

		int nrOfCtrlPoints = 0;
		in.read(reinterpret_cast<char*>(&nrOfCtrlPoints), sizeof(int));

		if (modelDebug)std::cout << nrOfCtrlPoints << std::endl;
		Model *model = new Model();
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
		//Read the materials for the mesh as well as the Texture file name
		int fileNameLength = 0;
		glm::vec3 diffuse;
		float specular = 0;

		std::string diffuseTexture = "";
		in.read(reinterpret_cast<char*>(&fileNameLength), sizeof(int));
		char *tempFileName;
		tempFileName = new char[fileNameLength];
		in.read(tempFileName, fileNameLength);
		diffuseTexture.append(tempFileName, fileNameLength);

		in.read(reinterpret_cast<char*>(&diffuse), sizeof(diffuse));
		in.read(reinterpret_cast<char*>(&specular), sizeof(specular));

		mesh->material.diffuseColour = glm::vec3(0.5,0.5,0.5);
		mesh->material.diffuseColour = diffuse;
		mesh->material.specularColour = glm::vec3(specular, specular, specular);
		mesh->material.ambientColour = glm::vec3(0.5,0.5,0.5);

		glm::vec3 pos;
		glm::vec3 scale;
		glm::vec3 rotation;
		//Read the position, rotation and scale values
		in.read(reinterpret_cast<char*>(&pos), sizeof(pos));
		in.read(reinterpret_cast<char*>(&rotation), sizeof(rotation));
		in.read(reinterpret_cast<char*>(&scale), sizeof(scale));

		model->setPos(pos);
		model->setScale(scale);
		model->setRotationMatrix(rotation);
		model->rotate();
		model->addMesh(mesh);
		model->setupModel();
		model->setBoundingSphereRadius();
		modelVector.push_back(model);
	}
	return true;
}
//Delete all models from memory
void Level::unloadModels()
{
	for (int i = 0; i < staticModels.size(); i++)
	{
		staticModels[i]->deleteMeshes();
		delete staticModels[i];
	}
	staticModels.clear();

	for (int i = 0; i < dynamicModels.size(); i++)
	{
		delete dynamicModels[i];
	}
	dynamicModels.clear();

	for (int i = 0; i < colliders.size(); i++)
	{
		colliders[i]->deleteMeshes();
		delete colliders[i];
	}
	colliders.clear();
}
//Sets the triggerboxes for this level
void Level::setupTriggers(Player* player)
{
	std::vector<glm::vec2> corners = { glm::vec2(3,2), glm::vec2(6,2), glm::vec2(3,0), glm::vec2(6,0) };
	TriggerSettings settings;
	settings.onEnter = true;
	settings.onExit = true;
	triggerBoxes.push_back(new Trigger(corners, settings, player, player, "endLevel"));

	//water land
	std::vector<glm::vec2> corners2 = { glm::vec2(9,5), glm::vec2(15,5), glm::vec2(9,0), glm::vec2(15,0) };
	TriggerSettings settings2;
	settings2.onEnter = true;
	settings2.onExit = true;
	triggerBoxes.push_back(new Trigger(corners2, settings2, player, player, "hellogais"));

	Model* heart = new Model("models/heart/HeartContainer.obj");
	createPickup(heart, glm::vec2(135, 110), "healthPickup" ,player);
	createPickup(heart, glm::vec2(145, 120), "healthPickup", player);
	createPickup(heart, glm::vec2(155, 130), "healthPickup", player);
	createPickup(heart, glm::vec2(165, 130), "healthPickup", player);
	createPickup(heart, glm::vec2(175, 130), "healthPickup", player);
	createPickup(heart, glm::vec2(185, 130), "healthPickup", player);
	createPickup(heart, glm::vec2(195, 130), "healthPickup", player);
	createPickup(heart, glm::vec2(205, 130), "healthPickup", player);
	createPickup(heart, glm::vec2(215, 130), "healthPickup", player);
}
void Level::updateTriggers(float dt)
{
	for (int i = 0; i < triggerBoxes.size(); i++)
	{
		triggerBoxes[i]->update(dt);
	}
	for (int i = 0; i < dynamicModels.size(); i++)
	{
		dynamicModels[i]->rotate();
	}
}
void Level::deleteTriggers()
{
	for (int i = 0; i < triggerBoxes.size(); i++)
	{
		delete triggerBoxes[i];
	}
	triggerBoxes.clear();
}
void Level::playMusic(SoundSystem* soundSystem)
{

}
void Level::stopMusic(SoundSystem* soundSystem)
{

}
//void Level::playMusic(SoundSystem *soundSystem)
//{
//	soundSystem->playMusic("audio/music/never.flac");
//}
//void Level::stopMusic(SoundSystem *soundSystem)
//{
//	soundSystem->stopMusic();
//}
//Getters
std::vector<Model*>& Level::getStaticModels()
{
	return staticModels;
}
std::vector<Model*>& Level::getCollisionBoxes()
{
	return colliders;
}
std::vector<Model*> Level::getDynamicModels()
{
	return dynamicModels;
}
std::vector<Trigger*> Level::getTriggers()
{
	return std::vector<Trigger*>();
}
glm::vec3 Level::getPlayerPos()
{
	return playerPos;
}
void Level::createPickup(Model* pickupModel, glm::vec2 position, std::string triggerName, Player* player)
{
	//health pickup
	glm::mat4 mat({
		0.5, 0, 0, 0,
		0, 0.5, 0, 0,
		0, 0, 0.5, 0,
		position.x, position.y, 0, 1
	});
	Model* thisPickup = new Model(pickupModel, mat);
	glm::vec3 min, max;
	thisPickup->getMinMaxBouding(min, max);
	min += glm::vec3(mat[3]);
	max += glm::vec3(mat[3]);
	thisPickup->setRotationMatrix(glm::rotate(glm::mat4(), glm::radians(-6.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	dynamicModels.push_back(thisPickup);

	std::vector<glm::vec2> corners3 = { glm::vec2(min), glm::vec2(min.x,max.y), glm::vec2(max), glm::vec2(max.x,min.y) };
	TriggerSettings settings3;
	settings3.onEnter = true;
	std::vector<GameObject*> allObjects;
	allObjects.push_back(player);
	allObjects.push_back(thisPickup);
	Trigger* pickupTrigger = new Trigger(thisPickup->getPoints(), settings3, player, allObjects, triggerName);
	triggerBoxes.push_back(pickupTrigger);
}
//Constructors
Level::Level()
{
	this->filePath = "";
	playerPos = glm::vec3(14,4,0);
}
Level::Level(std::string filePath)
{
	this->filePath = filePath;
	playerPos = glm::vec3(14, 4, 0);
}
//Destructor
Level::~Level()
{

}