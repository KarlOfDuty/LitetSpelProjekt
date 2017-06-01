#include "Level.h"

void Level::loadLevel(Player* player)
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
		if (modelDebug)std::cout << str << std::endl;
		if (str == "staticModels")
		{
			line >> path;
			readModels(path.c_str(), staticModels);
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
			if (showColliders)readModels(path.c_str(), staticModels);
		}
		else if (str == "enemies")
		{
			line >> path;
			readEnemies(path.c_str());
		}
		else if (str == "triggers")
		{
			line >> path;
			readTriggers(path.c_str(), triggerBoxes, player);
		}
	}
	file.close();
}
void Level::readTriggers(const char* filePath, std::vector<Trigger*> &vector, Player* player)
{
	//Temporary containers
	TriggerSettings settings;
	std::vector<GameObject*> activators = std::vector<GameObject*>();
	std::vector<GameObject*> targets = std::vector<GameObject*>();
	std::vector<std::string> commands;
	std::ifstream file(filePath);
	std::string str = "";
	int tempInt = 0;
	float tempFloat = 0.0f;
	//Gets a single line of the file at a time
	while (std::getline(file, str))
	{
		std::stringstream line;
		std::string path;
		//Takes the first word of the line and compares it to variable names
		line << str;
		line >> str;
		//New trigger
		if (str == "trigger")
		{
			//Read corners
			std::vector<glm::vec2> points = std::vector<glm::vec2>(4);
			for (int i = 0; i < 4; i++)
			{
				line >> points[i].x;
				line >> points[i].y;
				line >> str;
			}
			vector.push_back(new Trigger(points,settings,activators,targets,commands));
			//Reset variables for the next trigger
			settings = TriggerSettings();
			activators.clear();
			targets.clear();
			commands.clear();
		}
		//Activators
		else if (str == "activator")
		{
			line >> str;
			if (str == "player")
			{
				activators.push_back(player);
			}
			else if (str == "staticModel")
			{
				line >> tempInt;
				activators.push_back(staticModels[tempInt]);
			}
			else if (str == "dynamicModel")
			{
				line >> tempInt;
				activators.push_back(dynamicModels[tempInt]);
			}
			else if (str == "collider")
			{
				line >> tempInt;
				activators.push_back(colliders[tempInt]);
			}
			else if (str == "enemy")
			{
				line >> str;
				if (str == "all")
				{
					for (int i = 0; i < enemyList->getAllEnemies().size(); i++)
					{
						activators.push_back(enemyList->getAllEnemies()[i]);
					}
				}
				else
				{
					int index = std::stoi(str);
					activators.push_back(enemyList->getAllEnemies()[index]);
				}
			}
		}
		//Targets
		else if (str == "target")
		{
			line >> str;
			if (str == "player")
			{
				targets.push_back(player);
			}
			else if (str == "staticModel")
			{
				line >> tempInt;
				targets.push_back(staticModels[tempInt]);
			}
			else if (str == "dynamicModel")
			{
				line >> tempInt;
				targets.push_back(dynamicModels[tempInt]);
			}
			else if (str == "collider")
			{
				line >> tempInt;
				targets.push_back(colliders[tempInt]);
			}
			else if (str == "enemy")
			{
				line >> str;
				if (str == "all")
				{
					for (int i = 0; i < enemyList->getAllEnemies().size(); i++)
					{
						targets.push_back(enemyList->getAllEnemies()[i]);
					}
				}
				else
				{
					int index = std::stoi(str);
					targets.push_back(enemyList->getAllEnemies()[index]);
				}
			}
		}
		//Commands
		else if (str == "command")
		{
			line >> str;
			commands.push_back(str);
		}
		//Settings
		else if (str == "onEnter")
		{
			line >> tempInt;
			settings.onEnter = tempInt;
		}
		else if (str == "onEnterAll")
		{
			line >> tempInt;
			settings.onEnterAll = tempInt;
		}
		else if (str == "onExit")
		{
			line >> tempInt;
			settings.onExit = tempInt;
		}
		else if (str == "onExitAll")
		{
			line >> tempInt;
			settings.onExitAll = tempInt;
		}
		else if (str == "whileInside")
		{
			line >> tempInt;
			settings.whileInside = tempInt;
		}
		else if (str == "whileAllInside")
		{
			line >> tempInt;
			settings.whileAllInside = tempInt;
		}
		else if (str == "perActivator")
		{
			line >> tempInt;
			settings.perActivator = tempInt;
		}
		else if (str == "frequency")
		{
			line >> tempFloat;
			settings.frequency = tempFloat;
		}
		else if (str == "numberOfActivationsAllowed")
		{
			line >> tempInt;
			settings.numberOfActivationsAllowed = tempInt;
		}
		else if (str == "accociativeCommands")
		{
			line >> tempInt;
			settings.accociativeCommands = tempInt;
		}
		else if (str == "accociativeActions")
		{
			line >> tempInt;
			settings.accociativeActions = tempInt;
		}
	}
	file.close();
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
				mesh->vertices[(k * 3) + h].controllers = glm::ivec4(0);
				mesh->vertices[(k * 3) + h].weightsInfluence = glm::vec4(0);
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
				//mesh->vertices[(k * 3) + h].tangent = vec3;
				//Read the BiNormals for the primitive
				in.read(reinterpret_cast<char*>(&vec3), sizeof(vec3));
				if (modelDebug)
				{
					std::cout << "BiNormal: ";
					std::cout << vec3.x << " ";
					std::cout << vec3.y << " ";
					std::cout << vec3.z << std::endl;
				}
				//mesh->vertices[(k * 3) + h].biTangent = vec3;
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
				mesh->vertices[(k * 3) + h].texPos.x = vec2.x;
				mesh->vertices[(k * 3) + h].texPos.y = (vec2.y * -1.0f) + 1.0;
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
				if (modelDebug)std::cout << "'" << diffuseFileName << "'" << std::endl;
				mesh->material.textureMapDiffuseFile = diffuseFileName;
			}
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
		model->setPos(pos + glm::vec3(0, 0, -50));
		//Rotation
		glm::vec3 rotation;
		in.read(reinterpret_cast<char*>(&rotation), sizeof(rotation));
		model->setRotationMatrix(rotation);
		//Scale
		glm::vec3 scale;
		in.read(reinterpret_cast<char*>(&scale), sizeof(scale));
		model->setScale(scale);

		bool hasAnimation = false;
		in.read(reinterpret_cast<char*>(&hasAnimation), sizeof(bool));

		//Set up model
		model->rotate();
		model->addMesh(mesh);
		model->setupModel();
		model->setBoundingSphereRadius();
		modelVector.push_back(model);
	}
	in.close();
	return true;
}
bool Level::readEnemies(const char* filePath)
{
	//Temporary containers
	std::ifstream file(filePath);
	std::string str = "";
	//Gets a single line of the file at a time
	while (std::getline(file, str))
	{
		std::stringstream line;
		std::string type;
		line << str;
		line >> str;

		float x;
		float y;
		float z;
		line >> type;
		line >> x;
		line >> y;
		line >> z;
		if (type == "bats")
		{
			enemyList->createBatSwarm(glm::vec3(x, y, z));
		}
		else if (type == "boss")
		{
			enemyList->createBoss(glm::vec3(x, y, z));
		}
		else if (type == "crab")
		{
			enemyList->createCrab(glm::vec3(x, y, z));
		}
		else if (type == "firefly")
		{
			enemyList->createFirefly(glm::vec3(x, y, z));
		}
		else if (type == "bat")
		{
			enemyList->createGiantBat(glm::vec3(x, y, z));
		}
		else if (type == "skeleton")
		{
			enemyList->createSkeleton(glm::vec3(x, y, z), false);
		}
		else if (type == "slime")
		{
			enemyList->createSlime(glm::vec3(x, y, z));
		}
		else if (type == "toad")
		{
			enemyList->createToad(glm::vec3(x, y, z));
		}
	}
	file.close();
	return true;
}
bool Level::readTrigers(const char * filePath)
{
	//Temporary containers
	std::ifstream file(filePath);
	std::string str = "";
	//Gets a single line of the file at a time
	while (std::getline(file, str))
	{
		std::stringstream line;
		std::string type;
		line << str;
		line >> str;

		float x;
		float y;
		float z;
		line >> type;
		line >> x;
		line >> y;
		line >> z;
		if (type == "heart")
		{
			enemyList->createBatSwarm(glm::vec3(x, y, z));
		}
		else if (type == "boss")
		{
			enemyList->createBoss(glm::vec3(x, y, z));
		}
	}
	file.close();
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
	//Should probably be able to switch music track
}
void Level::stopMusic(SoundSystem* soundSystem)
{

}
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
	playerPos = glm::vec3(0, 0, 0);
}
Level::Level(std::string filePath, EnemyManager * enemy)
{
	this->enemyList = enemy;
	this->filePath = filePath;
	playerPos = glm::vec3(0, 2, 0);
}
//Destructor
Level::~Level()
{

}